#ifndef PROGRESSHELPER_H
#define PROGRESSHELPER_H

#include <QObject>

#include <QTime>
#include <QTimer>

class QLabel;
class QProgressBar;

class ObjectModel;
class ProcessTypeNames;

class ProgressHelper : public QObject
{
    Q_OBJECT
public:
    explicit ProgressHelper(QObject *parent = 0);

    void initialize(QLabel *elapsedTimeLabel
            , QLabel *processTypeNameLabel
            , QLabel *objectTypeNameLabel
            , QLabel *progressCountLabel
            , QProgressBar *progressBar
            , ObjectModel *model);

signals:

public slots:
    void beginBatch();
    void endBatch();

    void onTimeout();

    void onProgressStart(int type, int count);
    void onProgressChange(int type, int value);
    void onProgressEnd(int type);

    void setProcessTypeName(int type);

    void onCurrentObjectTypeChanged(int objectType);

private:
    QTime m_progressTime;
    QTimer m_progressTimer;
    ProcessTypeNames *m_processTypeNames;

    QLabel *m_elapsedTimeLabel;
    QLabel *m_processTypeNameLabel;
    QLabel *m_objectTypeNameLabel;
    QLabel *m_progressCountLabel;
    QProgressBar *m_progressBar;

    ObjectModel *m_model;
};

#endif // PROGRESSHELPER_H
