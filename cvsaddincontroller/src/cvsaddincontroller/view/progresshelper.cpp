#include "progresshelper.h"

#include <QApplication>
#include <QLabel>
#include <QProgressBar>

#include "processtypenames.h"
#include "cvsmodel/objectmodel.h"

#include "pch.hpp"

ProgressHelper::ProgressHelper(QObject *parent)
    : QObject(parent)
    , m_processTypeNames(new ProcessTypeNames(this))
{
    connect( &m_progressTimer, SIGNAL(timeout()), this, SLOT(onTimeout()) );
}

void ProgressHelper::initialize(QLabel *elapsedTimeLabel, QLabel *processTypeNameLabel, QLabel *progressCountLabel, QProgressBar *progressBar, ObjectModel *model)
{
    m_elapsedTimeLabel = elapsedTimeLabel;
    m_processTypeNameLabel = processTypeNameLabel;
    m_progressCountLabel = progressCountLabel;
    m_progressBar = progressBar;

    connect( model, SIGNAL(progressStart(int,int)),  this, SLOT(onProgressStart(int,int)) );
    connect( model, SIGNAL(progressChange(int,int)), this, SLOT(onProgressChange(int,int)) );
    connect( model, SIGNAL(progressEnd(int)),        this, SLOT(onProgressEnd(int)) );

    m_progressBar->reset();
}

void ProgressHelper::beginBatch()
{
    m_progressTime.restart();
    m_progressTimer.start(250);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_progressCountLabel->setText(tr("( 0000 / 0000 )"));
}

void ProgressHelper::endBatch()
{
    m_progressTimer.stop();
    onTimeout();
    QApplication::restoreOverrideCursor();
    m_progressCountLabel->setText(tr("( 0000 / 0000 )"));
}

void ProgressHelper::onTimeout()
{
    int msecs = m_progressTime.elapsed() % 1000;
    int secs = m_progressTime.elapsed() / 1000;
    int mins = (secs / 60) % 60;
    int hours = (secs / 3600);
    secs = secs % 60;

    m_elapsedTimeLabel->setText(
                QString("%1:%2:%3.%4")
                .arg(hours, 2, 10, QLatin1Char('0'))
                .arg(mins, 2, 10, QLatin1Char('0'))
                .arg(secs, 2, 10, QLatin1Char('0'))
                .arg(msecs, 3, 10, QLatin1Char('0'))
                );
}

void ProgressHelper::onProgressStart(int type, int count)
{
    m_progressBar->setRange(0, count);
    m_progressBar->setValue(0);
    setProcessTypeName(type);
}

void ProgressHelper::onProgressChange(int type, int value)
{
    Q_UNUSED(type);
    m_progressBar->setValue(value);
    m_progressCountLabel->setText(QString("( %1 / %2 )")
                                    .arg(value, 4, 10, QLatin1Char('0'))
                                    .arg(m_progressBar->maximum(), 4, 10, QLatin1Char('0'))
                                    );
    setProcessTypeName(type);
    QApplication::processEvents();
}

void ProgressHelper::onProgressEnd(int type)
{
    Q_UNUSED(type);
    m_progressBar->setValue(0);
    m_progressBar->setMaximum(1);
    m_progressBar->reset();
    m_processTypeNameLabel->setText("");
}

void ProgressHelper::setProcessTypeName(int type)
{
    m_processTypeNameLabel->setText(QString("%1 ...").arg( m_processTypeNames->value(type) ));
}

