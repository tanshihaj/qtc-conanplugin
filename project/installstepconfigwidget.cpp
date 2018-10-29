/** @author Kamil Zaripov
    @date 28.10.2018 */

#include "installstepconfigwidget.h"
#include "ui_installstepconfigwidget.h"
#include "installstep.h"
#include "constants.h"

using namespace Conan::Internal;
using namespace ProjectExplorer;

InstallStepConfigWidget::InstallStepConfigWidget(InstallStep *installStep)
    : BuildStepConfigWidget(),
      m_installStep(installStep),
      m_ui(new Ui::InstallStepConfigWidget())
{
    m_ui->setupUi(this);
    connect(m_installStep, &InstallStep::processCommandChanged,
            this, &InstallStepConfigWidget::updateUi);
    connect(m_installStep, &InstallStep::processArgumentsChanged,
            this, &InstallStepConfigWidget::updateUi);
    connect(m_ui->additionalArgumentsLineEdit, &QLineEdit::textEdited,
            m_installStep, &InstallStep::setAdditionalArguments);
    updateUi();
}

InstallStepConfigWidget::~InstallStepConfigWidget() {
    delete m_ui;
}

QString InstallStepConfigWidget::summaryText() const {
    auto resultCommand = QStringLiteral("<b>Conan:</b> ") + m_installStep->processParameters()->command() + QChar::Space + m_installStep->processParameters()->arguments();
    return resultCommand;
}

QString InstallStepConfigWidget::displayName() const {
    return tr(Constants::C_CONANINSTALLSTEPWIDGET_DISPLAY);
}

void InstallStepConfigWidget::updateUi() {
    m_ui->additionalArgumentsLineEdit->setText(m_installStep->additionalArguments());
    auto resultCommand = m_installStep->processParameters()->command() + QChar::Space + m_installStep->processParameters()->arguments();
    m_ui->resultCommandLineEdit->setText(resultCommand);
    updateSummary();
}
