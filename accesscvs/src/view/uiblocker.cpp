#include "uiblocker.h"

#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>

#include <QScreen>

class ProcessingDialog : public QDialog
{
    QLabel *m_label;
public:
    ProcessingDialog(QWidget *parent) : QDialog(parent)
    {
        setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint | Qt::SplashScreen );

        m_label = new QLabel(this);
        m_label->setAlignment( Qt::AlignVCenter | Qt::AlignHCenter );

        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(m_label);
        setLayout(layout);

        m_label->setStyleSheet("QLabel { color : white ;} ");
        setStyleSheet("* {background: qlineargradient( x1:0 y1:0, x2:1 y2:1, stop:0 #333333, stop:0.3 #000000, stop:0.7 #000000, stop:1 #333333);}");

        setWindowOpacity( 0.65 );
    }

    void setText(const QString &text)
    {
        m_label->setText(text);
    }
};


UiBlocker::UiBlocker(QWidget *parentWidget, QObject *parent)
    : QObject(parent)
    , m_parentWidget(parentWidget)
{
    for ( int i = 0 ; i < QApplication::screens().count() ; ++i )
    {
        m_dialogs.append( new ProcessingDialog(m_parentWidget) );
    }
}

void UiBlocker::show()
{
    for ( int i = 0 ; i < QApplication::screens().count() ; ++i )
    {
        QScreen *screen = QApplication::screens().at(i);
        ProcessingDialog *dialog = m_dialogs.at(i);

        dialog->setMinimumSize( screen->availableSize() );
        QRect rect = screen->availableGeometry();

        dialog->move( rect.left(), rect.top() );
    }

    foreach( ProcessingDialog *dialog, m_dialogs )
        dialog->show();
}

void UiBlocker::hide()
{
    foreach( ProcessingDialog *dialog, m_dialogs )
        dialog->hide();
}

void UiBlocker::setText(const QString &text)
{
    foreach( ProcessingDialog *dialog, m_dialogs )
        dialog->setText(text);
}

