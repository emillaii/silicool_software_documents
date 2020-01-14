#ifndef ACTIONERROR_H
#define ACTIONERROR_H

#include "silicolerror.h"

class ActionError : public SilicolError
{
public:
    ActionError(QString moduleName, QString errorMsg, QString suggestion = "");
};

class ActionErrorHandler : public ErrorHandler
{

    // ErrorHandler interface
protected:
    virtual bool handleCustomError(QString errHandling) override;
};

#endif    // ACTIONERROR_H
