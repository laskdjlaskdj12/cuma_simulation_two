#include <QCoreApplication>
#include <QTest>
#include "Cuma_Debug/cuma_debug.h"
#include "Test/cuma_client_test.h"
#include "Test/cuma_server_test.h"
#include "Test/cuma_main_unit_test.h"
#include "Test/cuma_file_test.h"
#include "Test/cuma_unit_base_test.h"

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    //각 해당 테스트 클래스들을 할당함
    //Cuma_File_test f_test;
    //Cuma_Main_Unit_test u_test;
    //Cuma_client_test c_test;
    //Cuma_server_test s_test;
    Cuma_Unit_Base_Test base_test;

    //QTest::qExec(&f_test);
    //QTest::qExec(&s_test);
    //QTest::qExec(&u_test);
    //QTest::qExec(&c_test);
    QTest::qExec(&base_test);

    a.exec();

}
