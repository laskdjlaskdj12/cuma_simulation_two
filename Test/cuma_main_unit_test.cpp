#include "cuma_main_unit_test.h"

Cuma_Main_Unit_test::Cuma_Main_Unit_test(QObject *parent)
{

}

void Cuma_Main_Unit_test::t_set_ulist_property()
{
    Cuma_Debug("set cache env ", __LINE__);

    //먼저 환경을 clear함
    env_clear_cache();

    Cuma_Debug("set QSharedPointer vector property", __LINE__);
    QVector<QSharedPointer<Cuma_Main> > p;
    //프로퍼티를 세팅함
    for(int i = 0; i < 10; i++)
    {
        QSharedPointer<Cuma_Main>  t = QSharedPointer<Cuma_Main>(new Cuma_Main);
        t->mf_set_pid(i);
        p.append(t);
    }
    Cuma_Debug("set unit_list", __LINE__);
    mf_set_unit_list(p);

    Cuma_Debug("compare unit list is same", __LINE__);
    QVERIFY(p == mf_get_unit_list());

    //메모리를 정리함
    for(int i = 0; i < 10; i++)
        p[i].clear();
}

void Cuma_Main_Unit_test::t_set_ping_limit_property()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ", __LINE__);
    env_clear_cache();

    Cuma_Debug("set uint32_t vector property", __LINE__);
    another_pid_ping_array ping_list;

    //핑리스트를 만듬
    //pid가 0인 유닛이 ping 리미트를 생성
    my_ping_list pid_1;
    pid_1.append(0);
    pid_1.append(10);

    //pid가 1인 유닛이 ping 리미트를 생성
    my_ping_list pid_2;
    pid_2.append(9);
    pid_2.append(0);


    ping_list.append(pid_1);
    ping_list.append(pid_2);

    Cuma_Debug("set unit_list", __LINE__);
    //ping 리미트 타임을 세팅함
    mf_set_dealy_lst(ping_list);

    Cuma_Debug("compare unit list is same", __LINE__);
    QVERIFY(mf_get_dealy_lst() == ping_list);

}

void Cuma_Main_Unit_test::t_is_active_property()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ", __LINE__);
    env_clear_cache();

    Cuma_Debug("set active", __LINE__);
    mf_set_active(true);

    Cuma_Debug("compare unit list is same", __LINE__);
    QVERIFY(mf_is_active() == true);
}

/*void Cuma_Main_Unit_test::t_s_stop_unit()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ", __LINE__);
    env_clear_cache();

    Cuma_Debug("set uint32_t vector property", __LINE__);

    QSignalSpy check_sig(this, SIGNAL(s_stop_unit()));

    mf_set_active(true);

    Cuma_Debug("emit signal", __LINE__);
    emit s_stop_unit();

    QCOMPARE(check_sig.count(), 1);
    Cuma_Debug("emit clear", __LINE__);

    QCOMPARE(mf_is_active(), true);
    Cuma_Debug("active is clear", __LINE__);
}*/

void Cuma_Main_Unit_test::t_s_recv()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ", __LINE__);
    env_clear_cache();

    Cuma_Debug("set QSignalSpy", __LINE__);
    QSignalSpy check_sig(this, SIGNAL(s_recv(QJsonObject)));

    Cuma_Debug("Generate recv protocol", __LINE__);
    QJsonObject t_protocol;
    t_protocol["test"] = true;

    //signal을 emit함
    Cuma_Debug("emit clear", __LINE__);
    emit this->s_recv(t_protocol);

    Cuma_Debug("check signal count", __LINE__);
    QCOMPARE(check_sig.count(), 1);
}

void Cuma_Main_Unit_test::t_s_start_command()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ", __LINE__);
    env_clear_cache();

    Cuma_Debug("set QSignalSpy", __LINE__);
    QSignalSpy check_sig(this, SIGNAL(s_start_command(QJsonObject)));

    Cuma_Debug("Generate recv protocol", __LINE__);
    QJsonObject t_protocol;
    t_protocol["test"] = true;

    //signal을 emit함
    Cuma_Debug("emit clear", __LINE__);
    emit this->s_start_command(t_protocol);

    Cuma_Debug("check signal count", __LINE__);
    QCOMPARE(check_sig.count(), 1);

}

void Cuma_Main_Unit_test::t_f_recv_process()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ", __LINE__);
    env_clear_cache();

    Cuma_Debug("set QSignalSpy", __LINE__);
    QSignalSpy check_sig(this, SIGNAL(s_recv(QJsonObject)));

    Cuma_Debug("Generate recv protocol", __LINE__);
    QJsonObject t_protocol;
    t_protocol["test"] = true;

    //signal을 emit함
    Cuma_Debug("emit clear", __LINE__);
    emit this->s_recv(t_protocol);

    Cuma_Debug("check signal count", __LINE__);
    QCOMPARE(check_sig.count(), 1);

    //recv의 프로세스에서 로그된 기록을 찾음
    Cuma_Debug("check log record of recv process", __LINE__);
    QVERIFY(mf_get_report_json().isEmpty() == false);
}

void Cuma_Main_Unit_test::t_f_tell_time()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ", __LINE__);
    env_clear_cache();

    Cuma_Debug("set uint32_t vector property", __LINE__);

    Cuma_Debug("check time is start", __LINE__);
    //시간이 시작되었는지 체크
    QVERIFY (unit_Timer::is_start == true);

}

void Cuma_Main_Unit_test::t_f_find_unit_from_inside_timeout_unit()
{
    //먼저 환경을  clear함
    Cuma_Debug("set cache env ", __LINE__);
    env_clear_cache();

    //타임아웃이 안된 유닛의 리스트에 있는 pid 로  QSharedPointer<Cuma_Main>를 리턴함


    //비교할 QSharedPointer<Cuma_Main> QVector 를 생성함
    Cuma_Debug("Generate compare pid_pointer", __LINE__);
    QVector<QSharedPointer<Cuma_Main>> t_compare_pid_pointer;

    //객체를 생성하여 포인터에 input함
    Cuma_Debug("make test unit obj env", __LINE__);
    for(int i = 0; i<10; i++)
    {
        QSharedPointer<Cuma_Main> m = QSharedPointer<Cuma_Main>(new Cuma_Main);
        m->mf_set_pid(i);
        m_Cuma_unit_inside_timeout_unit_list.append(m);
        t_compare_pid_pointer.append(m);
    }

    Cuma_Debug("compare unit and test obj is same ", __LINE__);
    QVERIFY (t_compare_pid_pointer == m_Cuma_unit_inside_timeout_unit_list);

    Cuma_Debug("check f_find_unit_from_inside_timeout_unit", __LINE__);
    for(int i = 0; i< 10; i++)
    {
        Cuma_Debug("check [" + QString::number(i) + "]", __LINE__);
        QVERIFY (f_find_unit_from_inside_timeout_unit(i) == t_compare_pid_pointer[i]);
    }

}

void Cuma_Main_Unit_test::t_f_find_unit_from_Cuma_unit_list()
{
    //먼저 환경을  clear함
    Cuma_Debug("set cache env ", __LINE__);
    env_clear_cache();

    //타임아웃이 안된 유닛의 리스트에 있는 pid 로  QSharedPointer<Cuma_Main>를 리턴함


    //비교할 QSharedPointer<Cuma_Main> QVector 를 생성함
    Cuma_Debug("Generate compare pid_pointer", __LINE__);
    QVector<QSharedPointer<Cuma_Main>> t_compare_pid_pointer;

    //객체를 생성하여 포인터에 input함
    Cuma_Debug("make test unit obj env", __LINE__);
    for(int i = 0; i<10; i++)
    {
        QSharedPointer<Cuma_Main> m = QSharedPointer<Cuma_Main>(new Cuma_Main);
        m->mf_set_pid(i);
        m_Cuma_unit_list.append(m);
        t_compare_pid_pointer.append(m);
    }

    Cuma_Debug("compare unit and test obj is same ", __LINE__);
    QVERIFY (t_compare_pid_pointer == m_Cuma_unit_list);

    Cuma_Debug("check f_find_unit_from_inside_timeout_unit", __LINE__);
    for(int i = 0; i< 10; i++)
    {
        Cuma_Debug("check [" + QString::number(i) + "]", __LINE__);
        QVERIFY (f_find_unit_from_Cuma_unit_list(i) == t_compare_pid_pointer[i]);
    }
}

void Cuma_Main_Unit_test::t_f_push_unit()
{
    //먼저 환경을  clear함
    Cuma_Debug("set cache env ", __LINE__);
    env_clear_cache();

    //타임아웃이 안된 유닛의 리스트에 있는 pid 로  QSharedPointer<Cuma_Main>를 리턴함


    //비교할 QSharedPointer<Cuma_Main> QVector 를 생성함
    Cuma_Debug("Generate compare pid_pointer", __LINE__);
    QVector<QSharedPointer<Cuma_Main>> t_compare_pid_pointer;

    //객체를 생성하여 포인터에 input함
    Cuma_Debug("make test unit obj env", __LINE__);
    for(int i = 0; i<10; i++)
    {
        QSharedPointer<Cuma_Main> m = QSharedPointer<Cuma_Main>(new Cuma_Main);
        m->mf_set_pid(i);
        f_push_unit(m);
        t_compare_pid_pointer.append(m);
    }

    Cuma_Debug("compare unit and test obj is same ", __LINE__);
    QVERIFY (t_compare_pid_pointer == m_Send_Unit_list);
}

void Cuma_Main_Unit_test::t_f_pop_unit()
{
    //먼저 환경을  clear함
    Cuma_Debug("set cache env ", __LINE__);
    env_clear_cache();

    //타임아웃이 안된 유닛의 리스트에 있는 pid 로  QSharedPointer<Cuma_Main>를 리턴함


    //비교할 QSharedPointer<Cuma_Main> QVector 를 생성함
    Cuma_Debug("Generate compare pid_pointer", __LINE__);
    QVector<QSharedPointer<Cuma_Main>> t_compare_pid_pointer;

    //객체를 생성하여 포인터에 input함
    Cuma_Debug("make test unit obj env", __LINE__);
    for(int i = 0; i<10; i++)
    {
        QSharedPointer<Cuma_Main> m = QSharedPointer<Cuma_Main>(new Cuma_Main);
        m->mf_set_pid(i);
        f_push_unit(m);
        t_compare_pid_pointer.append(m);
    }

    Cuma_Debug("compare unit and test obj is same ", __LINE__);
    QVERIFY (t_compare_pid_pointer == m_Send_Unit_list);

    Cuma_Debug("pop m_Send_Unit_list", __LINE__);
    for(int i = 9; i > -1 ; i--)
    {
        Cuma_Debug("compare f_pop_unit == t_copmare_pid_pointer[" + QString::number(i) + "]", __LINE__);
        QVERIFY(f_pop_unit() == t_compare_pid_pointer[i]);
    }
}

void Cuma_Main_Unit_test::t_f_f_save_recv_json_report()
{
    //먼저 환경을 clear함
    Cuma_Debug("set cache env ", __LINE__);
    env_clear_cache();

    Cuma_Debug("set QSignalSpy", __LINE__);
    QSignalSpy check_sig(this, SIGNAL(s_recv(QJsonObject)));

    Cuma_Debug("Generate recv protocol", __LINE__);
    QJsonObject t_protocol;
    t_protocol["test"] = true;

    //signal을 emit함
    Cuma_Debug("emit clear", __LINE__);
    emit this->s_recv(t_protocol);

    Cuma_Debug("check signal count", __LINE__);
    QCOMPARE(check_sig.count(), 1);

    //recv의 프로세스에서 로그된 기록을 찾음
    Cuma_Debug("check log record of recv process", __LINE__);
    QVERIFY(mf_get_report_json().isEmpty() == false);

    //로그된 기록을 출력함
    QJsonDocument doc(mf_get_report_json());
    Cuma_Debug("infomation of document" + doc.toJson());

}

void Cuma_Main_Unit_test::env_clear_cache()
{
    m_Unit_delay_time_array.clear();
    m_Cuma_unit_list.clear();
    m_Cuma_unit_inside_timeout_unit_list.clear();
    m_report_json.remove("recv");
    m_Send_Unit_list.clear();
    m_File->clear_binary();
    m_active = true;
    m_limit_bypass_count = 0;
    m_file_info_block.clear();
    m_file_frag_address.clear();

}

my_ping_list Cuma_Main_Unit_test::env_make_ping_list(uint32_t count)
{
    my_ping_list ret_arr;
    for(uint32_t j = 0; j < count; j++)
    {

        //아닐 경우 랜덤으로 만든 숫자를 넣음
        ret_arr.append( qrand()% 1000);
    }
    return ret_arr;
}

my_ping_list Cuma_Main_Unit_test::env_make_by_unit_ping_list(uint32_t pid, uint32_t count)
{
    my_ping_list ret_arr;
    for(uint32_t j = 0; j < count; j++)
    {

        if(j == pid)
        {
            ret_arr.append(0);
            continue;
        }

        //아닐 경우 랜덤으로 만든 숫자를 넣음
        ret_arr.append( qrand()% 1000);
    }
    return ret_arr;
}

another_pid_ping_array Cuma_Main_Unit_test::env_make_by_unit_ping_array(uint32_t count)
{
    another_pid_ping_array ret_arr;

    //유닛의 행렬 만큼 for문을 생성
    for(uint32_t i = 0; i < count; i++)
    {
        my_ping_list temp_list;
        for(uint32_t j = 0; j < count; j++)
        {
            //만약 i와 j와 같을시 0을 넣음
            if(i == j)
            {
                temp_list.append(0);
                continue;
            }

            //아닐 경우 랜덤으로 만든 숫자를 넣음
            temp_list.append( qrand()% 1000);
        }
        ret_arr.append(temp_list);
    }
    return ret_arr;
}
