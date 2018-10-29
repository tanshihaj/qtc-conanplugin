/** @author Kamil Zaripov
    @date 28.10.2018 */

#pragma once

#include <coreplugin/dialogs/ioptionspage.h>
#include <utils/fileutils.h>

#include <QPointer>

namespace Conan {
    namespace Internal {

        class SettingsWidget;

        class SettingsPage : public Core::IOptionsPage {
            Q_OBJECT

            Q_PROPERTY(Utils::FileName conanLocation READ conanLocation WRITE setConanLocation NOTIFY conanLocationChanged)

        public:
            explicit SettingsPage(QObject * parent = nullptr);

            QWidget * widget();
            void apply();
            void finish();

            Utils::FileName conanLocation() const;
            void setConanLocation(const Utils::FileName & conanLocation);

        signals:
            void conanLocationChanged(const Utils::FileName & conanLocation);

        private:
            void load();
            void save() const;

            QPointer<SettingsWidget> m_widget;

            Utils::FileName m_conanLocation;
        };

    }
}
