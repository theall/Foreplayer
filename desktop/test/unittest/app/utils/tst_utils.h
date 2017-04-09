#include <QString>
#include <QtTest>

class TestUtils : public QObject
{
    Q_OBJECT

public:
    TestUtils();

private slots:
    void testMiscroSecsToTimeStr();
    void testMiscroSecsToTimeStr_data();

    void testSecondsToTimeStr();
    void testSecondsToTimeStr_data();
};
