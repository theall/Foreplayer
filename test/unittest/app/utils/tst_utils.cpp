#include "tst_utils.h"

#include "../../utils/utils.h"

TestUtils::TestUtils()
{

}

void TestUtils::testMiscroSecsToTimeStr()
{
    QFETCH(long, micro_seconds);
    QFETCH(bool, pad_zero);
    QFETCH(QString, expect);

    Utils::microSecToTimeStr(micro_seconds, pad_zero);
}

void TestUtils::testMiscroSecsToTimeStr_data()
{
    QTest::addColumn<long>("micro_seconds");
    QTest::addColumn<bool>("pad_zero");
    QTest::addColumn<QString>("expect");

    QTest::newRow("micro1") << (long)72000 << true << "01:12";
    QTest::newRow("micro2") << (long)863999000 << true << "23:01:12";
    QTest::newRow("micro3") << (long)3599000 << true << "59:59";
    QTest::newRow("micro4") << (long)72000 << false << "1:12";
}

void TestUtils::testSecondsToTimeStr()
{
    QFETCH(long, micro_seconds);
    QFETCH(bool, pad_zero);
    QFETCH(QString, expect);

    Utils::microSecToTimeStr(micro_seconds, pad_zero);
}

void TestUtils::testSecondsToTimeStr_data()
{
    QTest::addColumn<long>("micro_seconds");
    QTest::addColumn<bool>("pad_zero");
    QTest::addColumn<QString>("expect");

    QTest::newRow("micro1") << (long)72 << true << "01:12";
    QTest::newRow("micro2") << (long)863999 << true << "23:01:12";
    QTest::newRow("micro3") << (long)3599 << true << "59:59";
    QTest::newRow("micro4") << (long)72000 << false << "1:12";
}
