#include <QCoreApplication>
#include <QTest>

#include "Test/cuma_main_unit_test.h"
#include "Test/cuma_file_test.h"

int main(int argc, char *argv[])
{
    //유닛테스트
    if(argc > 1 && argv[1] == "test")
    {
        return QTest::qExec()||QTest::qExec();
    }

    //실행
    else
    {
        QCoreApplication a(argc, argv);

        return a.exec();
    }
}
