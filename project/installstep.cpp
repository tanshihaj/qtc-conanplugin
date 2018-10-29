/** @author Kamil Zaripov
    @date 28.10.2018 */

#include "installstep.h"
#include "constants.h"
#include "installstepconfigwidget.h"
#include "settings/settingspage.h"

#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/project.h>
#include <projectexplorer/target.h>
#include <projectexplorer/projectnodes.h>
#include <utils/qtcassert.h>

using namespace Conan::Internal;
using namespace ProjectExplorer;
using namespace Utils;

const Project::NodeMatcher recepieMatcher = [](const Node * node) {
    return node->filePath().endsWith("conanfile.py") || node->filePath().endsWith("conanfile.txt");
};

InstallStep::InstallStep(BuildStepList * parentList)
    : AbstractProcessStep(parentList, Constants::C_CONANINSTALLSTEP_ID)
{
    setDefaultDisplayName(tr(Constants::C_CONANINSTALLSTEP_DISPLAY));
    setDisplayName(tr(Constants::C_CONANINSTALLSTEP_DISPLAY));

    auto bc = buildConfiguration();
    connect(bc, &BuildConfiguration::buildDirectoryChanged,
            this, &InstallStep::updateArguments);
    connect(bc->target()->project(), &ProjectExplorer::Project::fileListChanged,
            this, &InstallStep::updateRecepiePath);
    updateRecepiePath();
    updateCommand();
    updateArguments();
}

bool InstallStep::init(QList<const BuildStep *> & earlierSteps) {
    return AbstractProcessStep::init(earlierSteps);
}

BuildStepConfigWidget * InstallStep::createConfigWidget() {
    return new InstallStepConfigWidget(this);
}

bool InstallStep::fromMap(const QVariantMap & map) {
    AbstractProcessStep::fromMap(map);
    setAdditionalArguments(map[Constants::C_CONANINSTALLSTEP_ADDITIONALARGUMENTS].toString());
    return true;
}

QVariantMap InstallStep::toMap() const {
    QVariantMap result = AbstractProcessStep::toMap();
    result[Constants::C_CONANINSTALLSTEP_ADDITIONALARGUMENTS] = additionalArguments();
    return result;
}

Utils::FileName InstallStep::conanLocation() const {
    return m_conanLocation;
}

void InstallStep::setConanLocation(const Utils::FileName & conanLocation) {
    if (m_conanLocation == conanLocation)
        return;
    m_conanLocation = conanLocation;
    emit conanLocationChanged(m_conanLocation);
    updateCommand();
}

QString InstallStep::additionalArguments() const {
    return m_additionalArguments;
}

void InstallStep::setAdditionalArguments(const QString & additionalArguments) {
    if (m_additionalArguments == additionalArguments)
        return;
    m_additionalArguments = additionalArguments;
    emit additionalArgumentsChanged(m_additionalArguments);
    updateArguments();
}

FileName InstallStep::recepiePath() const {
    return m_recepiePath;
}

void InstallStep::setRecepiePath(const FileName & recepiePath) {
    if (m_recepiePath == recepiePath)
        return;
    m_recepiePath = recepiePath;
    emit recepiePathChanged(recepiePath);
    updateArguments();
}

void InstallStep::updateCommand() {
    processParameters()->setCommand(m_conanLocation.toString());
    emit processCommandChanged();
}

void InstallStep::updateArguments() {
    auto bc = buildConfiguration();
    QTC_ASSERT(bc, return);

    QStringList arguments;
    arguments << QStringLiteral("install");

    arguments << QStringLiteral("--install-folder=%1").arg(bc->buildDirectory().toString());
    arguments << QStringLiteral("--build=missing");
    arguments << QStringLiteral("--generator=qmake");

    arguments << m_additionalArguments;

    arguments << m_recepiePath.toString();

    processParameters()->setArguments(arguments.join(QChar::Space));
    emit processArgumentsChanged();
}

void InstallStep::updateRecepiePath() {
    const Utils::FileNameList recepitFiles = project()->files(recepieMatcher);
    if (!recepitFiles.isEmpty())
        setRecepiePath(recepitFiles.at(0));
}



InstallStepFactory::InstallStepFactory() {
    registerStep<InstallStep>(Constants::C_CONANINSTALLSTEP_ID);
    setDisplayName(tr("Conan install Step"));
    setSupportedStepList(ProjectExplorer::Constants::BUILDSTEPS_BUILD);
    setRepeatable(false);
}
