#include <QCoreApplication>

#include <QCommandLineParser>
#include <QDebug>

#include "accmodel/accmodel.h"
//#include "officelib/officelib.h"
#include <windows.h>

#include "pch.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();

    // version
    parser.addVersionOption();
    app.setApplicationVersion( APP_VERSION_STR );

    // decompile
    QCommandLineOption optDecompile(QStringList() << "d" << "decompile", QCoreApplication::translate("main", "Decompile"));
    parser.addOption(optDecompile);

    // compact repair
    QCommandLineOption optCompactRepair(QStringList() << "c" << "compact", QCoreApplication::translate("main", "Compact and Repair"));
    parser.addOption(optCompactRepair);

    // input file
    QCommandLineOption optInputFile(QStringList() << "i" << "input", QCoreApplication::translate("main", "Input file <inputfile>"), "inputfile");
    parser.addOption(optInputFile);

    // TODO: clearConnection()
    // TODO: removeLinkTables()
    // TODO: clearWorkTables()
    // TODO: export()
    // TODO: import()

    parser.process(app);

    bool isSetDecompile = parser.isSet(optDecompile);
    bool isSetCompactRepair = parser.isSet(optCompactRepair);
    QString inputFile = parser.value(optInputFile);

    qDebug() << "" << isSetDecompile;
    qDebug() << "" << isSetCompactRepair;
    qDebug() << "" << inputFile;

    if (inputFile.isEmpty())
    {
        qDebug() << QCoreApplication::translate("main", "inputfile doesn't exist : %1").arg(inputFile);
        return 1;
    }

    AccModel au;
    Access::Application *application = NULL;
    if (CoInitialize( NULL ) != S_OK )
        qDebug() << "CoInitialize is failed";


    if (isSetDecompile)
    {
        au.decompile(inputFile, 0);
    }

    if (isSetCompactRepair)
    {
        if (!application)
            application = au.createAccessApplication();

        au.compactRepair(application, inputFile, 1);
    }

    // clean-up
    if (application)
        au.quitAndDeleteApplication(application);

    CoUninitialize();
    return 0;
}
