#include "cuma_unit_base_test.h"

Cuma_Unit_Base_Test::Cuma_Unit_Base_Test(QObject *parent) : QObject(parent)
{
    Cuma_Debug::show_debug(true);
}

Cuma_Unit_Base_Test::~Cuma_Unit_Base_Test()
{

}

/*void Cuma_Unit_Base_Test::testBypassUnit()
{
    Cuma_Unit_Base unit_base;

    unit_base.set_Cuma_Unit_Ping_Timeout(5000);
    unit_base.init_Cuma_Unit(2, true);
    unit_base.init_Cuma_Unit_File_Frag_dir();

    QVERIFY (unit_base.set_target_unit( 0) == 0);
    QVERIFY (unit_base.start_all_unit_ping_spread() == 0);

    unit_base.get_target_unit()->mf_set_client_bypass_protocol_layer(true);
    QVERIFY (unit_base.get_target_unit()->mf_get_client_bypass_protocol_layer() == true);

    QSharedPointer<Cuma_Main> target_unit = unit_base.get_target_unit();
    target_unit->set_bypass_limit_count(1);

    Cuma_Debug("active Cuma_bypass protocol by target_uit");
    QVERIFY (unit_base.start_unit_bypass() == 0);

    QThread::sleep(2);

    QJsonObject report_json = target_unit->mf_get_report_json();

    Cuma_Debug("=============== unit : "  + QString::number(target_unit->mf_get_pid()) + "===============", __LINE__);
    Cuma_Debug(QJsonDocument(report_json).toJson(), __LINE__);

    QVERIFY (report_json["recv"].toArray().at(2).toObject().find("bypass_reply") != report_json["recv"].toArray().at(3).toObject().end());

    unit_base.finish_thread();
}
*/
void Cuma_Unit_Base_Test::testBypassPayload()
{
    Cuma_Unit_Base unit_base;

    unit_base.set_Cuma_Unit_Ping_Timeout(5000);
    unit_base.init_Cuma_Unit(3, true);
    unit_base.init_Cuma_Unit_File_Frag_dir();

    QVERIFY (unit_base.set_target_unit( 0) == 0);
    QVERIFY (unit_base.start_all_unit_ping_spread() == 0);

    unit_base.get_target_unit()->mf_set_client_bypass_protocol_layer(true);
    QVERIFY (unit_base.get_target_unit()->mf_get_client_bypass_protocol_layer() == true);

    QSharedPointer<Cuma_Main> target_unit = unit_base.get_target_unit();
    target_unit->set_bypass_limit_count(2);

    Cuma_Debug("active Cuma_bypass protocol by target_uit");
    QVERIFY (unit_base.start_unit_file_binary_exsist() == 0);

    QThread::sleep(2);

    QJsonObject report_json = target_unit->mf_get_report_json();

    Cuma_Debug("=============== unit : "  + QString::number(target_unit->mf_get_pid()) + "===============", __LINE__);
    Cuma_Debug(QJsonDocument(report_json).toJson(), __LINE__);

    QVERIFY (report_json["recv"].toArray().at(2).toObject().find("bypass_reply") != report_json["recv"].toArray().at(3).toObject().end());

    unit_base.finish_thread();
}
/*
void Cuma_Unit_Base_Test::testSetUnitCount()
{
    Cuma_Unit_Base unit_base;

    unit_base.set_Cuma_Unit_Count( 2);

    QVERIFY (unit_base.get_Cuma_Unit_Count() == 2);
}

void Cuma_Unit_Base_Test::testAllocUnit()
{
    Cuma_Unit_Base unit_base;

    unit_base.set_Cuma_Unit_Count( 2);

    QVERIFY (unit_base.alloc_unit() == 2);

    for(QSharedPointer<Cuma_Main> m : unit_base.get_alloc_unit())
    {
        QVERIFY (m.isNull() == false);
    }
}

void Cuma_Unit_Base_Test::testMakeUnitPid()
{
    Cuma_Unit_Base unit_base;

    QVERIFY (unit_base.make_pid(0).isEmpty() == true);
    QVERIFY (unit_base.make_pid(2).isEmpty() == false);

}

void Cuma_Unit_Base_Test::testMakeUnitPingLimit()
{
    Cuma_Unit_Base unit_base;

    QVERIFY (unit_base.make_Unit_Ping_Limit(0, 5000).isEmpty() == true);
    QVERIFY (unit_base.make_Unit_Ping_Limit(2, 5000).isEmpty() == false);

    QVector<QVector<uint32_t>> temp_ping_list_1 = unit_base.make_Unit_Ping_Limit(2, 5000);

    Cuma_Debug(" ============= compare 1 ============");
    foreach (const QVector<uint32_t>& u_list, temp_ping_list_1)
    {
        QStringList u_list_line;
        foreach (const uint32_t& t, u_list)
        {
            u_list_line.append(QString::number(t));
        }

        Cuma_Debug(u_list_line.join(" "), __LINE__);
    }

    QVector<QVector<uint32_t>> temp_ping_list_2 = unit_base.make_Unit_Ping_Limit(2, 5000);
    Cuma_Debug(" ============= compare 2 ============");
    foreach (const QVector<uint32_t>& u_list, temp_ping_list_2)
    {
        QStringList u_list_line;
        foreach (const uint32_t& t, u_list)
        {
            u_list_line.append(QString::number(t));
        }

        Cuma_Debug(u_list_line.join(" "), __LINE__);
    }

    QVERIFY (temp_ping_list_1 != temp_ping_list_2);
}

void Cuma_Unit_Base_Test::testDistributePingTimeoutTime()
{
    Cuma_Unit_Base unit_base;

    unit_base.set_Cuma_Unit_Count(100);
    unit_base.set_Cuma_Unit_Ping_Timeout( 5000);

    QMap<uint32_t, uint32_t> t = unit_base.distribute_Ping_Timeout_Time(unit_base.get_Cuma_Unit_Count(), 1000, false);

    QVERIFY(t.isEmpty() == false);

    QVERIFY (t.count() == unit_base.get_Cuma_Unit_Count());
}

void Cuma_Unit_Base_Test::testSetinitUnit()
{
    Cuma_Unit_Base unit_base;

    unit_base.set_Cuma_Unit_Count( 2);

    QVERIFY (unit_base.alloc_unit() == 2);

    QVERIFY (unit_base.init_unit() == 0);

    uint i = 0;
    for(i = 0; i < unit_base.get_alloc_unit().count(); i++)
    {
        QVERIFY2(unit_base.get_alloc_unit()[i]->mf_get_pid() == i, "Cuma_Main pointer :" + QString::number(unit_base.get_alloc_unit()[i]->mf_get_pid()).toUtf8() + " :is not same :" + QString::number(i).toUtf8());
    }
}

void Cuma_Unit_Base_Test::testSetUnitAttachThread()
{
    Cuma_Unit_Base unit_base;

    unit_base.set_Cuma_Unit_Count( 2);
    unit_base.alloc_unit();
    unit_base.init_unit();

    QVERIFY (unit_base.Attach_Cuma_Unit_to_Thread() == 0);

    foreach(QSharedPointer<QThread> i , unit_base.get_Cuma_Unit_Thread())
    {
        QVERIFY (i->isRunning() == false);
    }
}

void Cuma_Unit_Base_Test::start_all_unit_ping_test()
{
    //타겟 유닛을 통해서 모든 유닛에게 pid를 전송하는 역활을 함

    Cuma_Unit_Base unit_base;

    unit_base.set_Cuma_Unit_Ping_Timeout(5000);
    unit_base.init_Cuma_Unit(2);
    QVERIFY (unit_base.set_target_unit( 0) == 0);
    QVERIFY (unit_base.start_unit_ping_spread() == 0);

    for(QSharedPointer<QThread> m :unit_base.get_Cuma_Unit_Thread())
    {
        if(m->isFinished() == true || m->isRunning() == false)
        {
            Cuma_Error("Thread is not running", __LINE__).show_error_string();
        }
    }

        QVERIFY (unit_base.get_alloc_unit()[1]->mf_get_report_json().isEmpty() == false);

    QJsonObject log_report = unit_base.get_alloc_unit()[1]->mf_get_report_json();


    Cuma_Debug("=========== log_report_info ===========", __LINE__);
    Cuma_Debug(QJsonDocument(log_report).toJson(), __LINE__);

    QVERIFY (log_report["recv"].toArray().first().toObject()["From"].isNull() == false);

    unit_base.finish_thread();
}

void Cuma_Unit_Base_Test::start_unit_file_binary_spread_test()
{
    Cuma_Unit_Base unit_base;

    QFile file("test.txt");
    file.open(QFile::WriteOnly);
    for(uint i = 0; i < 10000; i++)
    {
        file.write(QString::number(INT64_MAX).toUtf8());
    }
    QByteArray file_binary = file.readAll();
    file.close();

    unit_base.set_Cuma_Unit_Ping_Timeout(5000);
    unit_base.init_Cuma_Unit(2, true);
    unit_base.init_Cuma_Unit_File_Frag_dir();
    QVERIFY (unit_base.set_target_unit( 0) == 0);
    QVERIFY (unit_base.set_target_to_read_File("test.txt", file_binary) == 0);
    QVERIFY (unit_base.start_all_unit_ping_spread() == 0);

    Cuma_Debug("=========== target_unit_ping_list_info ===========", __LINE__);
    for(QSharedPointer<Cuma_Main> m: unit_base.get_alloc_unit())
    {
        Cuma_Debug("unit : " + QString::number(m->mf_get_pid()) + " list", __LINE__);
        for(QSharedPointer<Cuma_Main> t_unit : unit_base.get_target_unit()->mf_t_get_unit_inside_timeout())
        {
            Cuma_Debug("pid : " + QString::number(t_unit->mf_get_pid()), __LINE__);
        }
    }

    QVERIFY (unit_base.start_unit_file_binary_spread() == 0);

    for(QSharedPointer<QThread> m :unit_base.get_Cuma_Unit_Thread())
    {
        if(m->isFinished() == true || m->isRunning() == false)
        {
            Cuma_Error("Thread is not running", __LINE__).show_error_string();
        }
    }

    Cuma_Debug("=========== log_report_info ===========", __LINE__);
    Cuma_Debug(QJsonDocument(unit_base.get_alloc_unit()[0]->mf_get_report_json()).toJson(), __LINE__);

    QDir tested_file_obj_dir = unit_base.get_alloc_unit()[1]->get_File_obj()->get_File_Directory();

    QStringList file_frag_dir_entry = tested_file_obj_dir.entryList();

    Cuma_Debug("=========== directory entry list ===========", __LINE__);
    Cuma_Debug(file_frag_dir_entry.join(","), __LINE__);
    QVERIFY (file_frag_dir_entry.count() > 2);

    unit_base.finish_thread();
}

void Cuma_Unit_Base_Test::testUnitRemoveAllFile()
{
    Cuma_Unit_Base unit_base;

    QFile file("test.txt");
    file.open(QFile::WriteOnly);
    for(uint i = 0; i < 10000; i++)
    {
        file.write(QString::number(INT64_MAX).toUtf8());
    }
    QByteArray file_binary = file.readAll();
    file.close();

    unit_base.set_Cuma_Unit_Ping_Timeout(5000);
    unit_base.init_Cuma_Unit(2, true);
    unit_base.init_Cuma_Unit_File_Frag_dir();
    QVERIFY (unit_base.set_target_unit( 0) == 0);
    QVERIFY (unit_base.set_target_to_read_File("test.txt", file_binary) == 0);
    unit_base.remove_all_unit_file();

    for(QSharedPointer<Cuma_Main > m: unit_base.get_alloc_unit())
    {
        QDir dir = m->get_File_obj()->get_File_Directory();
        dir.refresh();
        Cuma_Debug(dir.entryList().join(","), __LINE__);
        QVERIFY (dir.entryList().count() < 3);
    }

    unit_base.finish_thread();
}

void Cuma_Unit_Base_Test::start_unit_file_binary_exsist_test()
{
    Cuma_Unit_Base unit_base;

    QFile file("test.txt");
    file.open(QFile::WriteOnly);
    for(uint i = 0; i < 10000; i++)
    {
        file.write(QString::number(INT64_MAX).toUtf8());
    }
    QByteArray file_binary = file.readAll();
    file.close();

    unit_base.set_Cuma_Unit_Ping_Timeout(5000);
    unit_base.init_Cuma_Unit(2, true);
    unit_base.init_Cuma_Unit_File_Frag_dir();
    QVERIFY (unit_base.set_target_unit( 0) == 0);
    QVERIFY (unit_base.set_target_to_read_File("test.txt", file_binary) == 0);
    QVERIFY (unit_base.start_all_unit_ping_spread() == 0);
    QVERIFY (unit_base.start_unit_file_binary_spread() == 0);

    unit_base.get_target_unit()->get_File_obj()->set_File_Frag_Index(0);
    QVERIFY (unit_base.start_unit_file_binary_exsist() == 0);

    QVERIFY (unit_base.get_target_unit()->mf_get_report_json().isEmpty() == false);

    Cuma_Debug("=========== Debug binary_exsist_reply_protocol ===========", __LINE__);
    Cuma_Debug(QJsonDocument(unit_base.get_target_unit()->mf_get_report_json()).toJson(), __LINE__);

    QJsonArray target_unit_recv_log_array = unit_base.get_target_unit()->mf_get_report_json()["recv"].toArray();
    QJsonObject binary_exsist_reply_protocol = target_unit_recv_log_array.at(3).toObject();

    Cuma_Debug("=========== Debug binary_exsist_reply_protocol ===========", __LINE__);
    Cuma_Debug(QJsonDocument(binary_exsist_reply_protocol).toJson(), __LINE__);

    QVERIFY(binary_exsist_reply_protocol["process"].toString() == "check_file");
    QVERIFY(binary_exsist_reply_protocol["file_name"].toString() == "test.txt");
    QVERIFY(binary_exsist_reply_protocol["reply"].toBool() == true);

    unit_base.finish_thread();
}

void Cuma_Unit_Base_Test::start_unit_file_binary_request_test()
{
    Cuma_Unit_Base unit_base;

    unit_base.remove_all_unit_file();

    QFile file("test.txt");
    file.open(QFile::WriteOnly);
    for(uint i = 0; i < 10000; i++)
    {
        file.write(QString::number(INT64_MAX).toUtf8());
    }
    QByteArray file_binary = file.readAll();
    file.close();

    unit_base.set_Cuma_Unit_Ping_Timeout(5000);
    unit_base.init_Cuma_Unit(2, true);
    unit_base.init_Cuma_Unit_File_Frag_dir();
    QVERIFY (unit_base.set_target_unit( 0) == 0);
    QVERIFY (unit_base.set_target_to_read_File("test.txt", file_binary) == 0);
    QVERIFY (unit_base.start_all_unit_ping_spread() == 0);

    Cuma_Debug("=========== target_unit_ping_list_info ===========", __LINE__);
    for(QSharedPointer<Cuma_Main> m: unit_base.get_alloc_unit())
    {
        Cuma_Debug("unit : " + QString::number(m->mf_get_pid()) + " list", __LINE__);
        for(QSharedPointer<Cuma_Main> t_unit : unit_base.get_target_unit()->mf_t_get_unit_inside_timeout())
        {
            Cuma_Debug("pid : " + QString::number(t_unit->mf_get_pid()), __LINE__);
        }
    }

    QVERIFY (unit_base.start_unit_file_binary_spread() == 0);

    for(QSharedPointer<QThread> m :unit_base.get_Cuma_Unit_Thread())
    {
        if(m->isFinished() == true || m->isRunning() == false)
        {
            Cuma_Error("Thread is not running", __LINE__).show_error_string();
        }
    }


    Cuma_Debug("start unit_file_binary_request");
    QVERIFY (unit_base.start_unit_file_binary_request() == 0);

    QVERIFY (unit_base.get_target_unit()->get_File_obj()->read_file_frag("test.txt", 0) == Cuma_File::C_F_no_err);

    Cuma_Debug("=========== Debug binary_exsist_reply_protocol ===========", __LINE__);
    Cuma_Debug(QJsonDocument(unit_base.get_target_unit()->mf_get_report_json()).toJson(), __LINE__);

    unit_base.finish_thread();
}
*/
/*void Cuma_Unit_Base_Test::start_unit_bypass_test()
{
    Cuma_Unit_Base unit_base;

    unit_base.set_Cuma_Unit_Ping_Timeout(5000);
    unit_base.init_Cuma_Unit(2, true);
    unit_base.init_Cuma_Unit_File_Frag_dir();

    QVERIFY (unit_base.set_target_unit( 0) == 0);

    unit_base.get_target_unit()->mf_set_client_bypass_protocol_layer(true);
    QVERIFY (unit_base.get_target_unit()->mf_get_client_bypass_protocol_layer() == true);

    QVERIFY (unit_base.start_all_unit_ping_spread() == 0);

    QSharedPointer<Cuma_Main> target_unit = unit_base.get_target_unit();
    target_unit->set_bypass_limit_count(1);

    Cuma_Debug("active Cuma_bypass protocol by target_uit");
    QVERIFY (unit_base.start_unit_bypass() == 0);

    QThread::sleep(2);
    Cuma_Debug("=========== Debug binary_exsist_reply_protocol ===========", __LINE__);
    Cuma_Debug(QJsonDocument(target_unit->mf_get_report_json()).toJson(), __LINE__);

    unit_base.finish_thread();

}
*/
