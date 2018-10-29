/** @author Kamil Zaripov
    @date 28.10.2018 */

#pragma once

#include <utils/fileutils.h>

#include <QWidget>

namespace Conan {
    namespace Internal {

        class SettingsPage;
        namespace Ui { class SettingsWidget; }

        class SettingsWidget : public QWidget {
            Q_OBJECT
        public:
            SettingsWidget(SettingsPage * page, QWidget * parent = nullptr);
            ~SettingsWidget();

            void applySettings();

        private:
             void setConanPath();

        private:
            SettingsPage * m_page;
            Ui::SettingsWidget * m_ui;

            Utils::FileName m_conanLocation;
        };

    }
}
