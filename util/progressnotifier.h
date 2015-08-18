#ifndef PROGRESSNOTIFIER_H
#define PROGRESSNOTIFIER_H

#include <QObject>
#include <QReadWriteLock>

class ProgressNotifier : public QObject
{
    Q_OBJECT
public:
    explicit ProgressNotifier(int type, int count, QObject *parent);
    explicit ProgressNotifier(int type, QObject *parent);
    ~ProgressNotifier();

    void move(int value);
    void next();
    bool isFinished() const;
    int type() const;

signals:
    void start(int type, int count);
    void change(int type, int value);
    void end(int type);

public slots:
    void	progressRangeChanged(int minimum, int maximum);
    void	progressValueChanged(int progressValue);
    void	finished();

private:
    void init();
    void assign();

private:
    int m_type;
    int m_count;
    int m_value;
    bool m_finished;
    mutable QReadWriteLock m_lock;
};

#endif // PROGRESSNOTIFIER_H
