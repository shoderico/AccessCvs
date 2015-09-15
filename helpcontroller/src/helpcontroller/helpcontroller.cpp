#include "helpcontroller.h"

#include "comutil/comutil.h"

#include <QMessageBox>

HelpController::HelpController(QObject *parent)
    : QObject(parent)
    , m_application(0)
    , m_parentWidget(0)
{
}

HelpController::~HelpController()
{
}

void HelpController::initialize(Access::Application *application, QWidget *parentWidget)
{
    m_application = application;
    m_parentWidget = parentWidget;
}

QString HelpController::ribbonXml()
{
    QString content;
    content =
        "<group id=\"HelpGroup\" "
            "label=\"Help\"> "
            "<button id=\"HelpAboutButton\" "
            "   size=\"normal\" "
            "   label=\"About\" "
            "   onAction=\"ButtonClicked\" "
            "   getImage=\"GetButtonImage\" "
            "   /> "
        "</group>"
        ;
    return content;
}

bool HelpController::imagePath(const QString &controlId, QString &imagePath, QSize &size)
{
    Q_UNUSED(imagePath)
    Q_UNUSED(size)

    if ( controlId == "HelpAboutButton")
    {
//        imagePath = ":/images/manual.svg";
//        size = AddInUtil::ribbonIconSize(AddInUtil::Large);
    }
    else
        return false;

    return true;
}

bool HelpController::handleButtonClick(const QString &controlId)
{
    if (controlId == "HelpAboutButton")
        about();

    else
        return false;
    return true;
}

void HelpController::about()
{
    QMessageBox msg(m_parentWidget);
    msg.setWindowTitle(tr("About AccessCvs"));
    msg.setText(tr("<h2>AccessCvs 1.0.1</h2>"
                   "<p>Copyright &copy; 2015 shodeitz</p>"
                   ));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setIconPixmap( ComUtil::renderSvg(":/images/manual.svg", QSize(48,48)) );
    msg.exec();
}

