/** @author Kamil Zaripov
    @date 28.10.2018 */

#include "settingswidget.h"
#include "settingspage.h"
#include "ui_settingswidget.h"

#include <QDebug>

using namespace Conan::Internal;

SettingsWidget::SettingsWidget(SettingsPage * page, QWidget * parent)
    : QWidget(parent),
      m_page(page),
      m_ui(new Ui::SettingsWidget)
{
    m_ui->setupUi(this);

    m_ui->ConanLocationPathChooser->setPromptDialogTitle(tr("Select Conan executable Path"));
    m_ui->ConanLocationPathChooser->setPromptDialogFilter("conan");
    m_ui->ConanLocationPathChooser->setFileName(m_page->conanLocation());
    m_ui->ConanLocationPathChooser->setExpectedKind(Utils::PathChooser::File);
    connect(m_ui->ConanLocationPathChooser, &Utils::PathChooser::rawPathChanged,
            this, &SettingsWidget::setConanPath);
}

SettingsWidget::~SettingsWidget() {
    delete m_ui;
}

void SettingsWidget::applySettings() {
    m_page->setConanLocation(m_conanLocation);
}

void SettingsWidget::setConanPath() {
    m_conanLocation = Utils::FileName::fromUserInput(m_ui->ConanLocationPathChooser->rawPath());
}
