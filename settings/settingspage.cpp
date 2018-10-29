/** @author Kamil Zaripov
    @date 28.10.2018 */

#include "settingspage.h"
#include "settingswidget.h"
#include "constants.h"

#include <coreplugin/icore.h>
#include <utils/environment.h>
#include <utils/hostosinfo.h>
#include <projectexplorer/projectexplorerconstants.h>

#include <QFileInfo>

using namespace Conan::Internal;

using namespace ProjectExplorer;
using namespace Utils;

const QLatin1String SettingsGroup("ConanConfigurations");
const QLatin1String ConanLocationKey("ConanExecutableLocation");

SettingsPage::SettingsPage(QObject * parent)
    : Core::IOptionsPage(parent)
{
    setId(Constants::CONAN_SETTINGS_ID);
    setDisplayName(tr("Conan"));
    setCategory(ProjectExplorer::Constants::BUILD_AND_RUN_SETTINGS_CATEGORY);
    load();
}

QWidget * SettingsPage::widget() {
    if (!m_widget)
        m_widget = new SettingsWidget(this);
    return m_widget;
}

void SettingsPage::apply() {
    if (m_widget)
        m_widget->applySettings();
    save();
}

void SettingsPage::finish() {
    delete m_widget;
}

FileName SettingsPage::conanLocation() const {
    return m_conanLocation;
}

void SettingsPage::setConanLocation(const Utils::FileName & conanLocation) {
    if(m_conanLocation == conanLocation) { return; }
    m_conanLocation = conanLocation;
    emit conanLocationChanged(m_conanLocation);
}

void SettingsPage::load() {
    bool saveSettings = false;
    QSettings * settings = Core::ICore::settings();
    settings->beginGroup(SettingsGroup);

    m_conanLocation = FileName::fromString(settings->value(ConanLocationKey).toString());

    if (m_conanLocation.isEmpty()) {
        if (HostOsInfo::isLinuxHost() || HostOsInfo::isWindowsHost()) {
            Environment env = Environment::systemEnvironment();
            FileName location = env.searchInPath(QLatin1String("conan"));
            QFileInfo fi = location.toFileInfo();
            if (fi.exists() && fi.isExecutable() && !fi.isDir()) {
                m_conanLocation = location;
                saveSettings = true;
            }
        } else if (HostOsInfo::isMacHost()) {
            QFileInfo location(QLatin1String("/usr/local/bin/conan"));
            if (location.isExecutable() && !location.isDir()) {
                m_conanLocation = FileName::fromString(location.filePath());
                saveSettings = true;
            }
        }
    }

    settings->endGroup();

    if (saveSettings) {
        save();
    }
}

void SettingsPage::save() const {
    QSettings * settings = Core::ICore::settings();
    settings->beginGroup(SettingsGroup);
    settings->setValue(ConanLocationKey, m_conanLocation.toString());
    settings->endGroup();
}
