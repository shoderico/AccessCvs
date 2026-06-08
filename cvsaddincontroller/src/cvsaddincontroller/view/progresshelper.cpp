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
    , m_objectTypeNameLabel(0)
    , m_model(0)
{
    connect( &m_progressTimer, SIGNAL(timeout()), this, SLOT(onTimeout()) );
}

void ProgressHelper::initialize(QLabel *elapsedTimeLabel, QLabel *processTypeNameLabel, QLabel *objectTypeNameLabel, QLabel *progressCountLabel, QProgressBar *progressBar, ObjectModel *model)
{
    m_elapsedTimeLabel = elapsedTimeLabel;
    m_processTypeNameLabel = processTypeNameLabel;
    m_objectTypeNameLabel = objectTypeNameLabel;
    m_progressCountLabel = progressCountLabel;
    m_progressBar = progressBar;
    m_model = model;

    connect( model, SIGNAL(progressStart(int,int)),  this, SLOT(onProgressStart(int,int)) );
    connect( model, SIGNAL(progressChange(int,int)), this, SLOT(onProgressChange(int,int)) );
    connect( model, SIGNAL(progressEnd(int)),        this, SLOT(onProgressEnd(int)) );
    connect( model, SIGNAL(currentObjectTypeChanged(int)), this, SLOT(onCurrentObjectTypeChanged(int)) );

    m_progressBar->reset();
}

void ProgressHelper::beginBatch()
{
    m_progressTime.restart();
    m_progressTimer.start(250);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_progressCountLabel->setText(tr("( 0000 / 0000 )"));
    if (m_objectTypeNameLabel) m_objectTypeNameLabel->setText("");
}

void ProgressHelper::endBatch()
{
    m_progressTimer.stop();
    onTimeout();
    QApplication::restoreOverrideCursor();
    m_progressCountLabel->setText(tr("( 0000 / 0000 )"));
    if (m_objectTypeNameLabel) m_objectTypeNameLabel->setText("");
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
    if (m_objectTypeNameLabel) m_objectTypeNameLabel->setText("");
}

void ProgressHelper::setProcessTypeName(int type)
{
    m_processTypeNameLabel->setText(QString("%1 ...").arg( m_processTypeNames->value(type) ));
}

void ProgressHelper::onCurrentObjectTypeChanged(int objectType)
{
    if (!m_objectTypeNameLabel)
        return;
    QString name;
    if (m_model) {
        name = m_model->objectTypeDisplayName( static_cast<Model::ObjectType>(objectType) );
    }
    m_objectTypeNameLabel->setText( name.isEmpty() ? QString() : QString("%1 ...").arg(name) );
}

