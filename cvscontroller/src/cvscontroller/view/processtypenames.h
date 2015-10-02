#ifndef PROCESSTYPENAMES_H
#define PROCESSTYPENAMES_H

#include <QObject>
#include <QMap>

class ProcessTypeNames : public QObject
{
    Q_OBJECT
public:
    explicit ProcessTypeNames(QObject *parent = 0);

    QString value(const int type) const;

signals:

public slots:

private:
    QMap<int, QString> m_processTypeNames;
};

#endif // PROCESSTYPENAMES_H
