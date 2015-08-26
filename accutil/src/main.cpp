#include <QCoreApplication>

#include <QCommandLineParser>
#include <QDebug>

#include "accessutil/accessutil.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();

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

    if (isSetDecompile)
    {
        AccessUtil au;
        au.decompile(inputFile, 0);
    }

    if (isSetCompactRepair)
    {
        AccessUtil au;
        Access::Application *application = au.createAccessApplication();
        au.compactRepair(application, inputFile, 5);
        au.quitAndDeleteApplication(application);
    }

    return 0;
}
