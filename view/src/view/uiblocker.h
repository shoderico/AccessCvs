#ifndef UIBLOCKER_H
#define UIBLOCKER_H

#include "view_global.h"

#include <QObject>

class QDialog;
class QWidget;

class ProcessingDialog;

class VIEWSHARED_EXPORT UiBlocker : public QObject
{
    Q_OBJECT
public:
    explicit UiBlocker(QWidget *parentWidget, QObject *parent = 0);

signals:
public slots:
    void show();
    void hide();
    void setText(const QString &text);

private:
    QWidget *m_parentWidget;
    QList<ProcessingDialog *> m_dialogs;
};

#endif // UIBLOCKER_H
