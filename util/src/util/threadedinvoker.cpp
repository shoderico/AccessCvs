#include "threadedinvoker.h"

#include <QThread>
#include <QTimer>

ThreadedInvoker::ThreadedInvoker(const int msecLatency, QObject *parent)
    : QObject(parent)
    , m_latency(msecLatency)
    , m_thread( new QThread(this) )
{
    m_thread->start();
}

ThreadedInvoker::~ThreadedInvoker()
{
    m_thread->exit();
    m_thread->wait();
    delete m_thread;
}

void ThreadedInvoker::start(const QObject *receiver, const char *invokeMethod)
{
    Worker *worker = new Worker(m_latency);
    worker->moveToThread( m_thread );

    connect(worker, SIGNAL(started()), receiver, invokeMethod );
    connect(this, SIGNAL(finished()), worker, SLOT(deleteLater()) );

    QTimer::singleShot(m_latency, worker, SLOT(start()) );
}

void ThreadedInvoker::finish()
{
    QTimer::singleShot(m_latency, this, SIGNAL(finished()) );
}


void Worker::start()
{
    QTimer::singleShot(m_latency, this, SIGNAL(started()) );
}
