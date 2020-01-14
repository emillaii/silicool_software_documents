#include "actionerror.h"

ActionError::ActionError(QString moduleName, QString errorMsg, QString suggestion)
    : SilicolError(moduleName, errorMsg, suggestion)
{
    appendErrorHandlingOption(RETRY);
    appendErrorHandlingOption(IGNORE);
    appendErrorHandlingOption(ABORT);
}

bool ActionErrorHandler::handleCustomError(QString errHandling)
{
    Q_UNUSED(errHandling)
    return false;
}
