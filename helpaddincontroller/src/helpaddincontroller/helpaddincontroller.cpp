#include "helpaddincontroller.h"

#include "comutil/comutil.h"

#include <QMessageBox>
#include <QUuid>

#include "pch.hpp"

HelpAddinController::HelpAddinController(QObject *parent)
    : QObject(parent)
    , m_application(0)
    , m_parentWidget(0)
{
}

HelpAddinController::~HelpAddinController()
{
}

void HelpAddinController::initialize(QAxObject *application, QWidget *parentWidget)
{
    m_application = application;
    m_parentWidget = parentWidget;
}

QString HelpAddinController::ribbonXml()
{
    QString content;
    content =
        "<group id=\"HelpGroup." + QUuid::createUuid().toString().replace("{","").replace("}","") + "\" "
            "label=\"Help\"> "
            "<button id=\"HelpAboutButton." + QUuid::createUuid().toString().replace("{","").replace("}","") + "\" "
            "   size=\"normal\" "
            "   label=\"About\" "
            "   onAction=\"ButtonClicked\" "
            "   getImage=\"GetButtonImage\" "
            "   /> "
        "</group>"
        ;
    return content;
}

bool HelpAddinController::imagePath(const QString &controlId, QString &imagePath, QSize &size)
{
    Q_UNUSED(imagePath)
    Q_UNUSED(size)

    if ( controlId.startsWith("HelpAboutButton.") )
    {
//        imagePath = ":/images/manual.svg";
//        size = AddInUtil::ribbonIconSize(AddInUtil::Large);
    }
    else
        return false;

    return true;
}

bool HelpAddinController::handleButtonClick(const QString &controlId)
{
    if (controlId.startsWith("HelpAboutButton."))
        about();

    else
        return false;
    return true;
}

void HelpAddinController::about()
{
    QMessageBox msg(m_parentWidget);
    msg.setWindowTitle(tr("About AccessCvs"));
    msg.setText(tr("<h2>AccessCvs " APP_VERSION_STR  "</h2>"
                   "<p>Copyright &copy; 2026 shoderitz</p>"
                   ));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setIconPixmap( ComUtil::renderSvg(":/images/manual.svg", QSize(48,48)) );
    msg.exec();
}

