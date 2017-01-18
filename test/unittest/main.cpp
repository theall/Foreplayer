#include "app/utils/tst_utils.h"

int main(int argc, char *argv[])
{
    TestUtils tc;
    QTEST_SET_MAIN_SOURCE_PATH;
    QTest::qExec(&tc, argc, argv);
    return 0;
}
