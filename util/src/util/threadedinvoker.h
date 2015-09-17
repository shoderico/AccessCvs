#ifndef THREADEDINVOKER_H
#define THREADEDINVOKER_H

#include "util_global.h"

#include <QObject>

class QThread;

class UTIL_SHARED_EXPORT ThreadedInvoker : public QObject
{
    Q_OBJECT
public:
    explicit ThreadedInvoker(const int msecLatency, QObject *parent = 0);
    ~ThreadedInvoker();

    void start(const QObject *receiver, const char *invokeMethod);
    void finish();

signals:
    void started();
    void finished();

private:
    int m_latency;
    QThread *m_thread;
};


class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(const int msecLatency) : QObject(0), m_latency(msecLatency) {}
public slots:
    void start();
signals:
    void started();
private:
    int m_latency;
};


#endif // THREADEDINVOKER_H
