#include "placedut2trayerror.h"

placeDut2TrayError::placeDut2TrayError(QString moduleName, QString errorMsg, QString suggestion)
    : SilicolError(moduleName, errorMsg, suggestion)
{
    appendErrorHandlingOption(PLACE_WITHOUT_PR);
    appendErrorHandlingOption(RETRY);
}

placeDut2TrayErrorHandler::placeDut2TrayErrorHandler(SilicolFunc placeWithoutPrHandling)
{
    this->placeWithoutPrHandling = placeWithoutPrHandling;
}

bool placeDut2TrayErrorHandler::handleCustomError(QString errHandling)
{
    if (errHandling == PLACE_WITHOUT_PR)
    {
        placeWithoutPrHandling();
        return true;
    }
    return false;
}
