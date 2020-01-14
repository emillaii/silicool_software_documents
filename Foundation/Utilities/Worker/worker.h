#ifndef WORKER_H
#define WORKER_H

#include <QMap>
#include <QObject>
#include <functional>

// signal slot
// functional + concurrent
// static meta call
// action handle

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);

signals:

public slots:
    virtual void test() = 0;

private:
    std::function<void()> f;
    QMap<uint, std::function<void()>> funcs;
};

#endif    // WORKER_H
