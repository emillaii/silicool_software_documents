#include "placedut2jigerror.h"

PlaceDut2JigError::PlaceDut2JigError(QString moduleName, QString errorMsg, QString suggestion)
    : SilicolError(moduleName, errorMsg, suggestion)
{
    appendErrorHandlingOption(MANUALLY_TAKE_AWAY);
    appendErrorHandlingOption(MANUALLY_PUT_AWAY);
    appendErrorHandlingOption(RETRY);
}

PlaceDut2JigErrorHandler::PlaceDut2JigErrorHandler(SilicolFunc manuallyTakeAwayHandling,
                                                   SilicolFunc manuallyPutAwayHandling)
{
    this->manuallyTakeAwayHandling = manuallyTakeAwayHandling;
    this->manuallyPutAwayHandling = manuallyPutAwayHandling;
}

bool PlaceDut2JigErrorHandler::handleCustomError(QString errHandling)
{
    if (errHandling == MANUALLY_PUT_AWAY)
    {
        manuallyPutAwayHandling();
        return true;
    }
    else if (errHandling == MANUALLY_TAKE_AWAY)
    {
        manuallyTakeAwayHandling();
        return true;
    }
    else
    {
        return false;
    }
}
