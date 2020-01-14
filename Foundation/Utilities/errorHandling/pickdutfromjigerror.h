#ifndef PICKDUTFROMJIGERROR_H
#define PICKDUTFROMJIGERROR_H

#include "silicolerror.h"

class PickDutFromJigError : public SilicolError
{
public:
    PickDutFromJigError(QString moduleName, QString errorMsg, QString suggestion = "");
};

class PickDutFromJigErrorHandler : public ErrorHandler
{

public:
    PickDutFromJigErrorHandler(SilicolFunc manuallyTakeAwayHandling);
    // ErrorHandler interface
protected:
    virtual bool handleCustomError(QString errHandling) override;

private:
    SilicolFunc manuallyTakeAwayHandling;
};

#endif    // PICKDUTFROMJIGERROR_H
