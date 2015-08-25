#ifndef CONCURRENTMAPHELPER_H
#define CONCURRENTMAPHELPER_H

#include "util_global.h"

#include <QObject>
#include <QFutureWatcher>
#include <QEventLoop>
#include "progressnotifier.h"


template <typename T>
class UTILSHARED_EXPORT ConcurrentMapHelper // : public QObject
{
    //Q_OBJECT
    QFutureWatcher<T> m_futureWatcher;
    QEventLoop m_eventLoop;
    ProgressNotifier *m_progressNotifier;
public:
    ConcurrentMapHelper(ProgressNotifier *progressNotifier /*, QObject *parent = 0*/)
        : m_progressNotifier(progressNotifier)
    {
        QObject::connect( &m_futureWatcher, SIGNAL(finished()),                    &m_eventLoop,       SLOT(quit()) );
        QObject::connect( &m_futureWatcher, SIGNAL(finished()),                    m_progressNotifier, SLOT(finished()) );
        QObject::connect( &m_futureWatcher, SIGNAL(progressRangeChanged(int,int)), m_progressNotifier, SLOT(progressRangeChanged(int,int)) );
        QObject::connect( &m_futureWatcher, SIGNAL(progressValueChanged(int)),     m_progressNotifier, SLOT(progressValueChanged(int)) );
    }

    void run(const QFuture<T> &future )
    {
        m_futureWatcher.setFuture( future );
        if (!m_progressNotifier->isFinished())
            m_eventLoop.exec();
    }

private:
};

#endif // CONCURRENTMAPHELPER_H
