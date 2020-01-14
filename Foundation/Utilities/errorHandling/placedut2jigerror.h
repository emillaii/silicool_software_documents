#ifndef PLACEDUT2JIGERROR_H
#define PLACEDUT2JIGERROR_H

#include "silicolerror.h"

class PlaceDut2JigError : public SilicolError
{
public:
    PlaceDut2JigError(QString moduleName, QString errorMsg, QString suggestion = "");
};

class PlaceDut2JigErrorHandler : public ErrorHandler
{

public:
    PlaceDut2JigErrorHandler(SilicolFunc manuallyTakeAwayHandling, SilicolFunc manuallyPutAwayHandling);
    // ErrorHandler interface
protected:
    virtual bool handleCustomError(QString errHandling) override;

private:
    SilicolFunc manuallyTakeAwayHandling;
    SilicolFunc manuallyPutAwayHandling;
};

#endif    // PLACEDUT2JIGERROR_H
