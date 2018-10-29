/** @author Kamil Zaripov
    @date 28.10.2018 */

#pragma once

#include <projectexplorer/abstractprocessstep.h>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/buildsteplist.h>

namespace Conan {
    namespace Internal {

        class InstallStep : public ProjectExplorer::AbstractProcessStep {
            Q_OBJECT

        public:
            InstallStep(ProjectExplorer::BuildStepList * parentList);

            bool init(QList<const ProjectExplorer::BuildStep *> & earlierSteps) override;
            ProjectExplorer::BuildStepConfigWidget * createConfigWidget() override;

            bool fromMap(const QVariantMap & map) override;
            QVariantMap toMap() const override;

            Utils::FileName conanLocation() const;
            void setConanLocation(const Utils::FileName & conanLocation);

            QString additionalArguments() const;
            void setAdditionalArguments(const QString & additionalArguments);

            Utils::FileName recepiePath() const;
            void setRecepiePath(const Utils::FileName & recepiePath);

        signals:
            void conanLocationChanged(const Utils::FileName & conanLocation);
            void additionalArgumentsChanged(const QString & additionalArguments);
            void recepiePathChanged(const Utils::FileName & recepiePath);
            void processCommandChanged();
            void processArgumentsChanged();

        private:
            void updateCommand();
            void updateArguments();
            void updateRecepiePath();

            Utils::FileName m_conanLocation;
            QString m_additionalArguments;
            Utils::FileName m_recepiePath;
        };

        class InstallStepFactory : public ProjectExplorer::BuildStepFactory {
        public:
            InstallStepFactory();
        };

    }
}
