#include "picktrayduterror.h"

PickTrayDutError::PickTrayDutError(QString moduleName, QString errorMsg, QString suggestion)
    : SilicolError(moduleName, errorMsg, suggestion)
{
    appendErrorHandlingOption(RETRY);
    appendErrorHandlingOption(SKIP);
}

PickTrayDutErrorHandler::PickTrayDutErrorHandler(SilicolFunc skipHandling) : skipHandling(skipHandling) {}

bool PickTrayDutErrorHandler::handleCustomError(QString errHandling)
{
    if (errHandling == SKIP)
    {
        skipHandling();
        return true;
    }
    return false;
}
