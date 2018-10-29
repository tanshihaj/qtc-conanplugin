/** @author Kamil Zaripov
    @date 28.10.2018 */

#pragma once

#include <projectexplorer/buildstep.h>

namespace Conan {
    namespace Internal {

        class InstallStep;
        namespace Ui { class InstallStepConfigWidget; }

        class InstallStepConfigWidget : public ProjectExplorer::BuildStepConfigWidget {
            Q_OBJECT

        public:
            InstallStepConfigWidget(InstallStep * installStep);
            ~InstallStepConfigWidget() override;

            QString summaryText() const override;
            QString displayName() const override;

        private:
            void updateUi();

            InstallStep * m_installStep;
            Ui::InstallStepConfigWidget * m_ui;
        };

    }
}
