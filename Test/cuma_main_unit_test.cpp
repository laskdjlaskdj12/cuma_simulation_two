#include "cuma_main_unit_test.h"

Cuma_Main_Unit_test::Cuma_Main_Unit_test(QObject *parent)
{

}

void Cuma_Main_Unit_test::t_set_ulist_property()
{
    Cuma_Debug("set cache env ");

    //먼저 환경을 clear함
    env_clear_cache();

    Cuma_Debug("set QSharedPointer vector property");
    QVector<QSharedPointer<Cuma_Main> > p;
    //프로퍼티를 세팅함
    for(int i = 0; i < 10; i++)
    {
        QSharedPointer<Cuma_Main>  t = QSharedPointer<Cuma_Main>(new Cuma_Main);
        t->mf_set_pid(i);
        p.append(t);
    }
    Cuma_Debug("set unit_list");
    mf_set_unit_list(p);

    Cuma_Debug("compare unit list is same");
    QVERIFY(p == mf_get_unit_list());

    //메모리를 정리함
    for(int i = 0; i < 10; i++)
        p[i].clear();
}

void Cuma_Main_Unit_test::t_set_ping_limit_property()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ");
    env_clear_cache();

    Cuma_Debug("set uint32_t vector property");
    QVector<uint32_t> delay_list;
    for(int i = 0; i < 10; i++)
    {
        delay_list.append(i);
    }

    Cuma_Debug("set unit_list");
    //ping 리미트 타임을 세팅함
    mf_set_dealy_lst(delay_list);

    Cuma_Debug("compare unit list is same");
    QVERIFY(mf_get_dealy_lst() == delay_list);

}

void Cuma_Main_Unit_test::t_is_active_property()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ");
    env_clear_cache();

    Cuma_Debug("set active");
    mf_set_active(true);

    Cuma_Debug("compare unit list is same");
    QVERIFY(mf_is_active() == true);
}

void Cuma_Main_Unit_test::t_s_stop_unit()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ");
    env_clear_cache();

    Cuma_Debug("set uint32_t vector property");

    QSignalSpy check_sig(this, SIGNAL(s_stop_unit()));

    mf_set_active(true);

    Cuma_Debug("emit signal");
    emit s_stop_unit();

    QVERIFY(check_sig.wait());

    QCOMPARE(check_sig.count(), 1);
    Cuma_Debug("emit clear");

    QCOMPARE(mf_is_active(), true);
    Cuma_Debug("active is clear");
}

void Cuma_Main_Unit_test::t_s_recv()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ");
    env_clear_cache();

    Cuma_Debug("set QSignalSpy");
    QSignalSpy check_sig(this, SIGNAL(s_recv(QJsonObject)));

    Cuma_Debug("Generate recv protocol");
    QJsonObject t_protocol;
    t_protocol["test"] = true;

    //signal을 emit함
    Cuma_Debug("emit clear");
    emit this->s_recv(t_protocol);

    Cuma_Debug("check signal interruppte");
    QVERIFY(check_sig.wait());

    Cuma_Debug("check signal count");
    QCOMPARE(check_sig.count(), 1);
}

void Cuma_Main_Unit_test::t_s_start_command()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ");
    env_clear_cache();

    Cuma_Debug("set QSignalSpy");
    QSignalSpy check_sig(this, SIGNAL(s_start_command(QJsonObject)));

    Cuma_Debug("Generate recv protocol");
    QJsonObject t_protocol;
    t_protocol["test"] = true;

    //signal을 emit함
    Cuma_Debug("emit clear");
    emit this->s_start_command(t_protocol);

    Cuma_Debug("check signal interruppte");
    QVERIFY(check_sig.wait());

    Cuma_Debug("check signal count");
    QCOMPARE(check_sig.count(), 1);

}

void Cuma_Main_Unit_test::t_f_recv_process()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ");
    env_clear_cache();

    Cuma_Debug("set uint32_t vector property");
}

void Cuma_Main_Unit_test::t_f_tell_time()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ");
    env_clear_cache();

    Cuma_Debug("set uint32_t vector property");
}

void Cuma_Main_Unit_test::t_f_find_unit_from_inside_timeout_unit()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ");
    env_clear_cache();

    Cuma_Debug("set uint32_t vector property");
}

void Cuma_Main_Unit_test::t_f_find_unit_from_Cuma_unit_list()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ");
    env_clear_cache();

    Cuma_Debug("set uint32_t vector property");
}

void Cuma_Main_Unit_test::t_f_push_unit()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ");
    env_clear_cache();

    Cuma_Debug("set uint32_t vector property");
}

void Cuma_Main_Unit_test::t_f_pop_unit()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ");
    env_clear_cache();

    Cuma_Debug("set uint32_t vector property");
}

void Cuma_Main_Unit_test::t_f_send_ping_to_unit()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ");
    env_clear_cache();

    Cuma_Debug("set uint32_t vector property");
}

void Cuma_Main_Unit_test::t_f_f_save_recv_json_report()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ");
    env_clear_cache();

    Cuma_Debug("set uint32_t vector property");
}

void Cuma_Main_Unit_test::t_f_f_save_send_json_report()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ");
    env_clear_cache();

    Cuma_Debug("set uint32_t vector property");
}

void Cuma_Main_Unit_test::env_clear_cache()
{
    m_Unit_delay_time_array.clear();
    m_Cuma_unit_list.clear();
    m_Cuma_unit_inside_timeout_unit_list.clear();
    m_Send_Unit_list.clear();
    m_File->clear_binary();
    m_active = true;
    m_limit_bypass_count = 0;
    m_file_info_block.clear();
    m_file_frag_address.clear();
}
