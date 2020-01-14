#include "silicolerror.h"

SilicoolException::SilicoolException() : represent(""), hasPrintMsg(false) {}

SilicoolException::SilicoolException(const SilicoolException &other)
{
    this->represent = other.represent;
    this->hasPrintMsg = other.hasPrintMsg;
}

const char *SilicoolException::what()
{
    return represent.toUtf8();
}

void SilicoolException::setHasPrintMsg(bool value)
{
    this->hasPrintMsg = value;
}

SilicoolException::~SilicoolException()
{
    if (!hasPrintMsg)
    {
        qWarning(what());
    }
}

SilicolError::SilicolError(QString moduleName, QString errorMsg, QString suggestion)
    : moduleName(moduleName), errorMsg(errorMsg), suggestion(suggestion)
{
}

SilicolError::SilicolError(const SilicolError &other) : SilicoolException(other)
{
    this->moduleName = other.moduleName;
    this->errorMsg = other.errorMsg;
    this->errorHandlingOptions = other.errorHandlingOptions;
    this->suggestion = other.suggestion;
}

void SilicolError::appendErrorHandlingOption(QString option)
{
    errorHandlingOptions.append(option);
}

void SilicolError::createRepresent()
{
    represent = moduleName + " Error.\r\nError message: " + errorMsg
                + "\r\nAcceptable Error Handling: " + combineString(",", errorHandlingOptions)
                + (suggestion.isEmpty() ? "" : "\r\nSuggestion: " + suggestion);
}

SilicolAbort::SilicolAbort(QString abortMsg) : abortMsg(abortMsg) {}

SilicolAbort::SilicolAbort(const SilicolAbort &other) : SilicoolException(other)
{
    this->abortMsg = other.abortMsg;
}

void SilicolAbort::createRepresent()
{
    represent = abortMsg;
}

void ErrorHandler::handleError(SilicolFunc func)
{
    bool hasCountHandleContinuousFailure = false;
    while (true)
    {
        try
        {
            func();
            nContinuousFailure = 0;
            return;
        }
        catch (SilicolError &se)
        {
            qCritical() << "Catch error:" << se.what();
            if (!hasCountHandleContinuousFailure)
            {
                nContinuousFailure++;
                hasCountHandleContinuousFailure = true;
            }
            if (handleContinuousFailure && nContinuousFailure >= nMaxContinuoutFailre)
            {
                continuousFailureHandler();
                nContinuousFailure = 0;
                return;
            }
            QString errHandling;
            if (autoHandleError)
            {
                errHandling = autoHandleOption;
            }
            else
            {
                QString title = se.moduleName + " Error";
                QString content = se.errorMsg;
                if (!se.suggestion.isEmpty())
                {
                    content += "\r\nSuggestion: " + se.suggestion;
                }
                errHandling
                    = UIOperation::getIns()->getUIResponse(title, content, MsgBoxIcon::Error, se.errorHandlingOptions);
            }
            if (errHandling == ABORT)
            {
                SilicolAbort sa("User selected abort!");
                throw sa;
            }
            else if (errHandling == IGNORE)
            {
                qWarning("User selected ignore!");
                return;
            }
            else if (errHandling == RETRY)
            {
                continue;
            }
            else
            {
                if (handleCustomError(errHandling))
                {
                    return;
                }
                else
                {
                    throw SilicolAbort(QString("Unknown error handling: %1").arg(errHandling));
                }
            }
        }
    }
}

void ErrorHandler::setHandlingContinuousFailure(int nMaxContinuoutFailre, SilicolFunc continuousFailureHandler)
{
    CHECK_POINTER(continuousFailureHandler)
    this->nMaxContinuoutFailre = nMaxContinuoutFailre;
    this->continuousFailureHandler = continuousFailureHandler;
    this->handleContinuousFailure = true;
}

void ErrorHandler::setAutoHandleError(QString autoHandleOption)
{
    this->autoHandleOption = autoHandleOption;
}

void ErrorHandler::enableAutoHandleError(bool enable)
{
    if (enable && autoHandleOption == nullptr)
    {
        qFatal("Please setAutoHandleError first!");
    }
    this->autoHandleError = enable;
}

ErrorHandler::~ErrorHandler() {}
