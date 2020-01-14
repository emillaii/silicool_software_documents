#include "pickdutfromjigerror.h"

PickDutFromJigError::PickDutFromJigError(QString moduleName, QString errorMsg, QString suggestion)
    : SilicolError(moduleName, errorMsg, suggestion)
{
    appendErrorHandlingOption(MANUALLY_TAKE_AWAY);
    appendErrorHandlingOption(RETRY);
}

PickDutFromJigErrorHandler::PickDutFromJigErrorHandler(SilicolFunc manuallyTakeAwayHandling)
{
    this->manuallyTakeAwayHandling = manuallyTakeAwayHandling;
}

bool PickDutFromJigErrorHandler::handleCustomError(QString errHandling)
{
    if (errHandling == MANUALLY_TAKE_AWAY)
    {
        manuallyTakeAwayHandling();
        return true;
    }
    return false;
}
