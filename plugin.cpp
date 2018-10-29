/** @author Kamil Zaripov
    @date 28.10.2018 */

#include "plugin.h"
#include "constants.h"
#include "project/installstep.h"
#include "settings/settingspage.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <extensionsystem/pluginmanager.h>
#include <projectexplorer/project.h>
#include <projectexplorer/target.h>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/deployconfiguration.h>
#include <projectexplorer/runconfiguration.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/projectnodes.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>
#include <QListWidget>
#include <QMetaObject>

using namespace Conan;
using namespace Conan::Internal;

namespace Conan {
    namespace Internal {

        class ConanPluginPrivate {
        public:
            InstallStepFactory installStepFactory;
            SettingsPage settingsPage;
        };

    }
}

const auto recepieMatcher = [](const ProjectExplorer::Node * node) {
    return node->filePath().endsWith("conanfile.py") || node->filePath().endsWith("conanfile.txt");
};

const auto buildStepFinder = [](const ProjectExplorer::BuildStep * build) -> bool {
    return build->id() == Constants::C_CONANINSTALLSTEP_ID;
};

ConanPlugin::ConanPlugin() {
}

ConanPlugin::~ConanPlugin() {
    delete d;
}

bool ConanPlugin::initialize(const QStringList & arguments, QString * errorString) {
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    d = new ConanPluginPrivate;

    connect(&(d->settingsPage), &SettingsPage::conanLocationChanged,
            this, &ConanPlugin::updateConanLocation);

    auto sessionManager = ProjectExplorer::SessionManager::instance();
    connect(sessionManager, &ProjectExplorer::SessionManager::projectAdded,
            this, &ConanPlugin::addProject);
    connect(sessionManager, &ProjectExplorer::SessionManager::projectRemoved,
            this, &ConanPlugin::removeProject);
    for(auto project : sessionManager->projects()) {
        addProject(project);
    }

    return true;
}

void ConanPlugin::addProject(ProjectExplorer::Project * project) {
    connect(project, &ProjectExplorer::Project::fileListChanged,
            this, [this, project](){ this->handleProject(project); });
    handleProject(project);
}

void ConanPlugin::removeProject(ProjectExplorer::Project * project) {
    disconnect(project, nullptr, this, nullptr);
}

void ConanPlugin::handleProject(ProjectExplorer::Project * project) {
    const auto & recepies = project->files(recepieMatcher);
    const auto recipiesFound = !recepies.isEmpty();

    const auto & tragets = project->targets();
    for(auto target : tragets) {
        auto buildConfigs = target->buildConfigurations();
        for(auto buildConfig : buildConfigs) {
            auto stepList = buildConfig->stepList(ProjectExplorer::Constants::BUILDSTEPS_BUILD);
            if(recipiesFound) {
                auto steps = stepList->steps(buildStepFinder);
                if(steps.empty()) {
                    auto step = d->installStepFactory.create(stepList, Constants::C_CONANINSTALLSTEP_ID);
                    auto conanInstallStep = dynamic_cast<InstallStep*>(step);
                    if(conanInstallStep) {
                        conanInstallStep->setConanLocation(d->settingsPage.conanLocation());
                    }
                    stepList->insertStep(0, step);
                } else {
                    for(auto step : steps) {
                        auto conanInstallStep = dynamic_cast<InstallStep*>(step);
                        if(conanInstallStep) {
                            conanInstallStep->setConanLocation(d->settingsPage.conanLocation());
                        }
                    }
                }
            } else {
                for(int i = stepList->count()-1; i>=0; --i) {
                    if(stepList->at(i)->id() == Constants::C_CONANINSTALLSTEP_ID) {
                        stepList->removeStep(i);
                    }
                }
            }
        }
    }
}

void ConanPlugin::updateConanLocation() {
    auto sessionManager = ProjectExplorer::SessionManager::instance();
    for(auto project : sessionManager->projects()) {
        for(auto target : project->targets()) {
            auto buildConfigs = target->buildConfigurations();
            for(auto buildConfig : buildConfigs) {
                auto stepList = buildConfig->stepList(ProjectExplorer::Constants::BUILDSTEPS_BUILD);
                auto steps = stepList->steps(buildStepFinder);
                for(auto step : steps) {
                    auto conanInstallStep = dynamic_cast<InstallStep*>(step);
                    if(conanInstallStep) {
                        conanInstallStep->setConanLocation(d->settingsPage.conanLocation());
                    }
                }
            }
        }
    }
}

