#include <QString>
#include <QtTest>
#include <QCoreApplication>

class FileUtilTest : public QObject
{
    Q_OBJECT

public:
    FileUtilTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1_data();
    void testCase1();
};

FileUtilTest::FileUtilTest()
{
}

void FileUtilTest::initTestCase()
{
}

void FileUtilTest::cleanupTestCase()
{
}

void FileUtilTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

void FileUtilTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(false, "Failure");
}

QTEST_MAIN(FileUtilTest)

#include "tst_fileutiltest.moc"
