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
            , QLabel *progressCountLabel
            , QProgressBar *progressBar
            , ObjectModel *model);

signals:

public slots:
    void beginBatch();
    void endBatch();

    void onTimeout();

    void progressStart(int type, int count);
    void progressChange(int type, int value);
    void progressEnd(int type);

    void setProcessTypeName(int type);

private:
    QTime m_progressTime;
    QTimer m_progressTimer;
    ProcessTypeNames *m_processTypeNames;

    QLabel *m_elapsedTimeLabel;
    QLabel *m_processTypeNameLabel;
    QLabel *m_progressCountLabel;
    QProgressBar *m_progressBar;
};

#endif // PROGRESSHELPER_H
