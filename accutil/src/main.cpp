#include <QCoreApplication>

#include <QCommandLineParser>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();

    //
    QCommandLineOption optDecompile(QStringList() << "d" << "decompile", QCoreApplication::translate("main", "Decompile"));
    parser.addOption(optDecompile);

    //
    QCommandLineOption optCompactRepair(QStringList() << "c" << "compact", QCoreApplication::translate("main", "Compact and Repair"));
    parser.addOption(optCompactRepair);

    //
    QCommandLineOption optInputFile(QStringList() << "i" << "input", QCoreApplication::translate("main", "Input file <inputfile>"), "inputfile");
    parser.addOption(optInputFile);


    parser.process(app);

    bool isSetDecompile = parser.isSet(optDecompile);
    bool isSetCompactRepair = parser.isSet(optCompactRepair);
    QString inputFile = parser.value(optInputFile);

    qDebug() << "" << isSetDecompile;
    qDebug() << "" << isSetCompactRepair;
    qDebug() << "" << inputFile;

    return app.exec();
}
