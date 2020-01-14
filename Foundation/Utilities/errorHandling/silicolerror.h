#ifndef SILICOLERROR_H
#define SILICOLERROR_H

#include "../commonmethod.h"
#include "../uiHelper/uioperation.h"
#include "errorhandlingoption.h"
#include <exception>

typedef std::function<void()> SilicolFunc;

class ErrorHandler;

class SilicoolException
{
public:
    SilicoolException();

    SilicoolException(const SilicoolException &other);

    const char *what();

    void setHasPrintMsg(bool value);

    virtual ~SilicoolException();

protected:
    QString represent;
    virtual void createRepresent() = 0;

private:
    bool hasPrintMsg;
};

class SilicolError : public SilicoolException
{
public:
    SilicolError(QString moduleName, QString errorMsg, QString suggestion = "");
    SilicolError(const SilicolError &other);

protected:
    void appendErrorHandlingOption(QString option);

    void createRepresent() override;

private:
    friend class ErrorHandler;

    QString moduleName;
    QString errorMsg;
    QList<QString> errorHandlingOptions;
    QString suggestion;
};

class SilicolAbort : public SilicoolException
{
public:
    SilicolAbort(QString abortMsg);
    SilicolAbort(const SilicolAbort &other);

    // SilicoolException interface
protected:
    virtual void createRepresent() override;

private:
    QString abortMsg;
};

class ErrorHandler
{
public:
    void handleError(SilicolFunc func);

    void setHandlingContinuousFailure(int nMaxContinuoutFailre, SilicolFunc continuousFailureHandler);

    void setAutoHandleError(QString autoHandleOption);

    void enableAutoHandleError(bool enable);

    virtual ~ErrorHandler();

protected:
    ///
    /// \brief handleCustomError
    /// Need to be reimplemented.
    /// If you handled this option, return true.
    /// \param errHandling
    /// \return
    ///
    virtual bool handleCustomError(QString errHandling) = 0;

private:
    bool handleContinuousFailure = false;
    int nContinuousFailure = 0;
    int nMaxContinuoutFailre = 5;
    SilicolFunc continuousFailureHandler = nullptr;
    bool autoHandleError = false;
    QString autoHandleOption = nullptr;
};

#endif    // SILICOLERROR_H
