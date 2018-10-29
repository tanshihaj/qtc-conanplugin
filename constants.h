/** @author Kamil Zaripov
    @date 28.10.2018 */

#pragma once

#include <QtGlobal>
#include <QString>

namespace Conan {
    namespace Constants {

        const char CONAN_SETTINGS_ID[] = "BB.Conan Configurations";

        const char ACTION_ID[] = "Conan.Action";
        const char MENU_ID[] = "Conan.Menu";

        // ConanInstallStep
        const char C_CONANINSTALLSTEP_ID[] = "Conan.ConanInstallStep";
        const char C_CONANINSTALLSTEP_DISPLAY[] = QT_TRANSLATE_NOOP("ConanInstallStepFactory", "Conan install Step");
        const QString C_CONANINSTALLSTEP_ADDITIONALARGUMENTS = QStringLiteral("Conan.ConanInstallStep.AdditionalArguments");

        // ConanInstallStepWidget
        const char C_CONANINSTALLSTEPWIDGET_DISPLAY[] = QT_TRANSLATE_NOOP("ConanInstallStepWidget", "Conan install step");
        const char C_CONANINSTALLSTEPWIDGET_SUMMARY[] = QT_TRANSLATE_NOOP("ConanInstallStepWidget", "Conan install step");

    }
}
