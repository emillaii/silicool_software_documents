#ifndef PLACEDUT2TRAYERROR_H
#define PLACEDUT2TRAYERROR_H

#include "silicolerror.h"

class placeDut2TrayError : public SilicolError
{
public:
    placeDut2TrayError(QString moduleName, QString errorMsg, QString suggestion = "");
};

class placeDut2TrayErrorHandler : public ErrorHandler
{

public:
    placeDut2TrayErrorHandler(SilicolFunc placeWithoutPrHandling);
    // ErrorHandler interface
protected:
    virtual bool handleCustomError(QString errHandling) override;

private:
    SilicolFunc placeWithoutPrHandling;
};

#endif    // PLACEDUT2TRAYERROR_H
