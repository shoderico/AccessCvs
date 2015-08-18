#include "concurrentmaphelper.h"

//template <typename T>
//ConcurrentMapHelper<T>::ConcurrentMapHelper(ProgressNotifier *progressNotifier /*, QObject *parent*/)
//    : m_progressNotifier(progressNotifier)
//{
//    QObject::connect( &m_futureWatcher, SIGNAL(finished()),                    &m_eventLoop,       SLOT(quit()) );
//    QObject::connect( &m_futureWatcher, SIGNAL(finished()),                    m_progressNotifier, SLOT(finished()) );
//    QObject::connect( &m_futureWatcher, SIGNAL(progressRangeChanged(int,int)), m_progressNotifier, SLOT(progressRangeChanged(int,int)) );
//    QObject::connect( &m_futureWatcher, SIGNAL(progressValueChanged(int)),     m_progressNotifier, SLOT(progressValueChanged(int)) );
//}

//template <typename T>
//void ConcurrentMapHelper<T>::run(const QFuture<T> &future)
//{
//    m_futureWatcher.setFuture( future );
//    if (!m_progressNotifier->isFinished())
//        m_eventLoop.exec();
//}

