#include <QCoreApplication>
#include <QTest>
#include "Cuma_Unit_Base/cuma_unit_base.h"
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
    //Cuma_Unit_Base_Test base_test;

    //QTest::qExec(&f_test);
    //QTest::qExec(&s_test);
    //QTest::qExec(&u_test);
    //QTest::qExec(&c_test);
    //QTest::qExec(&base_test);


    Cuma_Unit_Base unit_base;
    Cuma_Debug::show_debug(true);

    unit_base.set_Cuma_Unit_Ping_Timeout(5000);
    unit_base.init_Cuma_Unit(100);
    unit_base.init_Cuma_Unit_File_Frag_dir();

    unit_base.set_target_unit( 0);
    unit_base.start_all_unit_ping_spread();

    unit_base.get_target_unit()->mf_set_client_bypass_protocol_layer(true);
    unit_base.get_target_unit()->mf_get_client_bypass_protocol_layer();

    QSharedPointer<Cuma_Main> target_unit = unit_base.get_target_unit();
    target_unit->set_bypass_limit_count(90);

    Cuma_Debug("active Cuma_bypass protocol by target_uit");
    unit_base.start_unit_file_binary_exsist("test.txt", 0);

    QThread::sleep(2);

    //모든 유닛들이 stop명령이 true일때까지 stop
    while(unit_base.wait_until_unit_is_finish() == true);

    QJsonObject report_json = target_unit->mf_get_report_json();

    Cuma_Debug("=============== unit : "  + QString::number(target_unit->mf_get_pid()) + "===============", __LINE__);
    Cuma_Debug(QJsonDocument(report_json).toJson(), __LINE__);

    unit_base.finish_thread();


    a.exec();

}
