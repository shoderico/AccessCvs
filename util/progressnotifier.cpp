#include "progressnotifier.h"

ProgressNotifier::ProgressNotifier(int type, int count, QObject *parent)
    : QObject(parent)
    , m_type(type)
    , m_count(count)
    , m_value(0)
    , m_finished(false)
{
    init();
}

ProgressNotifier::ProgressNotifier(int type, QObject *parent)
    : QObject(parent)
    , m_type(type)
    , m_count(0)
    , m_value(0)
    , m_finished(false)
{
    init();
}

ProgressNotifier::~ProgressNotifier()
{
    emit end(m_type);
}

void ProgressNotifier::move(int value)
{
    emit change(m_type, value);
    m_value = value;
}

void ProgressNotifier::next()
{
    m_value++;
    emit change(m_type, m_value);
}

bool ProgressNotifier::isFinished()
{
    //QMutexLocker locker(&m_mutex);
    QReadLocker locker(&m_lock);
    return m_finished;
}

void ProgressNotifier::progressRangeChanged(int minimum, int maximum)
{
    //QMutexLocker locker(&m_mutex);
    emit start(m_type, m_count);
}

void ProgressNotifier::progressValueChanged(int progressValue)
{
    int value = 0;
    {
        //QMutexLocker locker(&m_mutex);
        QWriteLocker locker(&m_lock);
        m_value++;
        value = progressValue;
    }
    emit change(m_type, value);
}

void ProgressNotifier::finished()
{
    {
        //QMutexLocker locker(&m_mutex);
        QWriteLocker locker(&m_lock);
        m_finished = true;
    }
    emit end(m_type);
}

void ProgressNotifier::init()
{
    assign();
    emit start(m_type, m_count);
}

void ProgressNotifier::assign()
{
    connect(this, SIGNAL(start(int,int)),   parent(), SIGNAL(progressStart(int,int)) );
    connect(this, SIGNAL(change(int,int)),  parent(), SIGNAL(progressChange(int,int)) );
    connect(this, SIGNAL(end(int)),         parent(), SIGNAL(progressEnd(int)) );
}



