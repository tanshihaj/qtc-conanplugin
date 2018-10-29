/** @author Kamil Zaripov
    @date 28.10.2018 */

#pragma once

#include "conan_global.h"

#include <extensionsystem/iplugin.h>
#include <projectexplorer/session.h>

namespace Conan {
    namespace Internal {

        class ConanPlugin : public ExtensionSystem::IPlugin {
            Q_OBJECT
            Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Conan.json")

        public:
            ConanPlugin();
            ~ConanPlugin();

            bool initialize(const QStringList & arguments, QString * errorString);
            void extensionsInitialized() {}
//            ShutdownFlag aboutToShutdown();

        private:
            void addProject(ProjectExplorer::Project * project);
            void removeProject(ProjectExplorer::Project * project);
            void handleProject(ProjectExplorer::Project * project);
            void updateConanLocation();

            class ConanPluginPrivate * d = nullptr;
        };

    }
}
