#ifndef PROGRESSNOTIFIER_H
#define PROGRESSNOTIFIER_H

#include <QObject>

class ProgressNotifier : public QObject
{
    Q_OBJECT
public:
    explicit ProgressNotifier(int type, int count, QObject *parent);
    explicit ProgressNotifier(int type, QObject *parent);
    ~ProgressNotifier();

    void move(int value);
    void next();

signals:
    void start(int type, int count);
    void change(int type, int value);
    void end(int type);

private:
    void init();
    void assign();

private:
    int m_type;
    int m_count;
    int m_value;
};

#endif // PROGRESSNOTIFIER_H
