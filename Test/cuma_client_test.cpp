#include "cuma_client_test.h"

Cuma_client_test::Cuma_client_test(QObject *parent)
{
    Cuma_Debug::show_debug(true);

    root_path = QDir::currentPath();

    //현재 테스트 디렉토리를 생성
    QDir t_dir;
    if(t_dir.exists("test_dir") == false)
    {
        t_dir.mkdir("test_dir");
    }

    t_dir.cd("test_dir");

    QDir::setCurrent(t_dir.absolutePath());

    m_File->init_dir();
}

Cuma_client_test::~Cuma_client_test()
{
    //테스트 디렉토리 삭제
    QDir t_dir;

    if(t_dir.currentPath() == root_path + "/test_dir")
    {
        //테스트 디렉토리 삭제
        t_dir.cdUp();
        t_dir.remove("test_dir");
    }

    //디렉토리 위치를 현재 디렉토리로 변환
    QDir::setCurrent(root_path);
}

/*void Cuma_client_test::t_mf_command_set_file_name()
{
    //환경을 clear
    env_clear_env();

    //파일 이름커맨드를 emit함
    QJsonObject c_proto;

    c_proto["command_set_file_name"] = "test.txt";
    mf_command_set_file_name("test.txt");

    Cuma_Debug("check command result");
    QVERIFY(m_File->get_File_Name() == "test.txt");

}
*/
/*void Cuma_client_test::t_mf_command_set_unit_bypass_count()
{
    //환경을 clear
    env_clear_env();

    //바이패스 카운트를 세팅
    QSignalSpy s_comm_file(this, SIGNAL(s_start_command(QJsonObject)));
    QJsonObject c_proto;

    c_proto["command_set_unit_bypass_count"] = 10;
    mf_command_set_unit_bypass_count(10);

    Cuma_Debug("check command result");
    QVERIFY(m_limit_bypass_count == 10);
}
*/
/*void Cuma_client_test::t_mf_command_ping_test()
{
    //환경 clear
    Cuma_Debug("clear env");
    env_clear_env();

    //테스트 유닛 생성
    QVector<QSharedPointer<Cuma_Main>> t_unit_list = env_make_unit(3);
    t_unit_list.append(QSharedPointer<Cuma_Main>(this));

    //테스트 유닛 pid와 리스트 등록
    env_set_test_env_unit(t_unit_list, 100);

    //테스트 유닛이 등록이되었는지 확인
    for(int i = 0; i < 4; i++)
    {
        QVERIFY(t_unit_list[i]->mf_get_unit_list().count() == t_unit_list.count());
        QVERIFY(t_unit_list[i]->mf_get_dealy_lst()[i].count() == t_unit_list.count());
    }

    //타임아웃 유닛 리스트의 큐를 비움
    m_Cuma_unit_inside_timeout_unit_list.clear();

    //테스트 유닛의 핑 리미트를 설정
    m_ping_limit = 50;

    //허용해야할 유닛의 리스트
    QVector<uint32_t> compare_unit = env_allow_ping_unit(m_ping_limit, mf_get_dealy_lst()[m_Pid], m_Pid);

    //허용 유닛 리스트를 출력
    for(uint32_t t : compare_unit)
    {
        Cuma_Debug("allow unit list : " + QString::number(t));
    }

    //유닛을 ping 테스트 시작함
    mf_command_ping_test();

    //스크리밍이 된 ping 리스트가 있는지 확인
    Cuma_Debug("unit inside_timeout_unit_list count = " + QString::number(m_Cuma_unit_inside_timeout_unit_list.count()), __LINE__);
    for(QSharedPointer<Cuma_Main> p : m_Cuma_unit_inside_timeout_unit_list)
    {
        Cuma_Debug("unit_pid = [" + QString::number(p->mf_get_pid()) + "]", __LINE__);
    }

    Cuma_Debug("compare return unit count = " + QString::number(compare_unit.count()));

    QVERIFY (m_Cuma_unit_inside_timeout_unit_list.count() == compare_unit.count());

    //해당 pid가 m_Cuma_unit_inside_timeout_unit_list에 있는지 확인
    for(uint32_t i :compare_unit)
    {
        QVERIFY(env_find_pid(i, m_Cuma_unit_inside_timeout_unit_list));
    }
}
*/
/*void Cuma_client_test::t_mf_command_spread_test()
{
    //모든 유닛에게 파일 frag를 전송하는 테스트

    //테스트 유닛 초기화
    env_clear_env();

    QVector<QSharedPointer<Cuma_Main>> t_unit_list = env_make_unit(3);
    t_unit_list.append(QSharedPointer<Cuma_Main>(this));

    env_set_test_env_unit(t_unit_list, 100);

    //파일 읽기
    QVERIFY (env_make_file("test.txt") == 0);

    QByteArray t_file_binary = env_get_file_binary("test.txt");
    m_File->set_File_Binary(t_file_binary);
    m_File->set_File_Name("test.txt");

    //Cuma_Frag_dir에 있는 모든 파일들을 삭제
    Cuma_Debug("delete Cuma_Frag_dir file", __LINE__);
    Cuma_Debug("current path : " + QDir::currentPath(), __LINE__);

    Cuma_Debug("m_Cuma_unit_inside_timeout_unit_list count : " + QString::number(m_Cuma_unit_inside_timeout_unit_list.count()));
    QVERIFY (m_Cuma_unit_inside_timeout_unit_list.count() == m_Unit_delay_time_array.count());

    QVERIFY (mf_command_spread_test() == 0);

    Cuma_Debug("check file frag count is same as t_unit_list.cout()", __LINE__);
    Cuma_Debug("file frag count = " + QString::number(m_File->get_File_Frag().count()), __LINE__);

    QVERIFY (m_File->get_File_Frag().count() == t_unit_list.count() || m_File->get_File_Frag().count() == t_unit_list.count() + 1);

    QDir dir;
    dir.cd("Cuma_Frag_dir");
    QStringList dir_lst = dir.entryList();

    for(QString s : dir_lst)
    {
        qDebug()<<s;
    }
}
*/
/*void Cuma_client_test::t_mf_command_req_file_test()
{
    //모든 유닛에게 bypass을 하는 테스트

    //테스트 유닛 초기화
    env_clear_env();

    QVector<QSharedPointer<Cuma_Main>> t_unit_list = env_make_unit(3);
    t_unit_list.append(QSharedPointer<Cuma_Main>(this));

    env_set_test_env_unit(t_unit_list, 100);

    //테스트 파일 생성
    QVERIFY (env_make_file("test.txt") == 0);

    //파일을 frag 생성
    env_get_file_frag("test.txt", t_unit_list.count());

    QVector<QByteArray> arr = env_get_file_frag("test.txt", t_unit_list.count());

    //테스트 파일 frag 을 유닛에게 저장함
    for(int i = 0; i < t_unit_list.count(); i++)
    {
        QSharedPointer<Cuma_File> f = t_unit_list[i]->get_File_obj();
        f->save_File_Frag(arr[i], "test.txt", i);

        QVector<uint32_t> frag_by_unit_address;
        frag_by_unit_address.append(t_unit_list[i]->mf_get_pid());

        QMap<uint32_t, QVector<uint32_t>>& file_block_list = m_file_frag_address["test.txt"];
        file_block_list[i] = frag_by_unit_address;
    }

    Cuma_Debug("================== End of test env set ==============", __LINE__);


    m_File->set_File_Name("test.txt");

    Cuma_Debug("================== start test ==============", __LINE__);

    //실행
    QVERIFY (mf_command_req_file_test() == 0);

    //테스트 유닛에 파일 frag가 맞는지 확인
    QVERIFY(m_file_frag_address["test.txt"].count() == 4);
}
*/
void Cuma_client_test::t_mf_command_trace_pass_test()
{
    //bypass가 실행시 해당 bypass tree가 구성이 되어있는지 확인함

    //테스트 유닛 초기화
    env_clear_env();

    QVector<QSharedPointer<Cuma_Main>> t_unit_list = env_make_unit(3);
    t_unit_list.append(QSharedPointer<Cuma_Main>(this));

    env_set_test_env_unit(t_unit_list, 100);

    //bypass 리미트 카운트 설정
    mf_command_set_unit_bypass_count(4);

    Cuma_Debug("========= start test =======", __LINE__);
    //bypass
    mf_command_trace_pass_test();

    //bypass reply 큐가 있는지 확인
    QVERIFY(m_bypass_protocol_queue.count() > 0);

    Cuma_Debug("========= check bypass protocol queue =======", __LINE__);
    Cuma_Debug(QJsonDocument(m_bypass_protocol_queue[0]).toJson(), __LINE__);

    //바이패스 array의 카운트가 0보다 큰지 확인
    QVERIFY (m_bypass_protocol_queue[0]["bypass"].toArray().count() > 0 );
    QVERIFY (m_bypass_protocol_queue[0]["bypass"].toArray().count() == 4 );


    //들어있는 bypass array를 출력함
    Cuma_Debug(QJsonDocument(m_bypass_protocol_queue[0]).toJson());
}

int Cuma_client_test::env_set_test_env_unit(QVector<QSharedPointer<Cuma_Main> > &v, uint32_t max_timeout)
{
    //pid와 unlit_list들의 기본적인 부분을 추가함
    int it_pid = 0;
    for(QSharedPointer<Cuma_Main> it: v)
    {
        it->mf_set_pid(it_pid);
        it->mf_set_unit_list(v);
        it->mf_t_set_limit_unit(v);
        it_pid++;
    }

    QVector<QVector<uint32_t>> unit_ping_list;

    for(int i = 0; i < v.size() ; i++)
    {
        QVector<uint32_t> ping_list;
        ping_list.resize(v.size());
        unit_ping_list.append(ping_list);
    }

    qsrand(1000);
    for(uint i = 0; i < v.count(); i++)
    {
        for(uint j = 0; j < v.count(); j++)
        {
            if(i == j)
            {
                unit_ping_list[i].replace(j, 0);
                continue;
            }

            else
            {
                uint32_t rand_num = ((uint32_t)qrand() % max_timeout);

                //[i][j]에 넣기
                unit_ping_list[i].replace(j, rand_num);

                //[j][i]에 넣기
                unit_ping_list[j].replace(i, rand_num);
            }
        }
    }

    //딜레이 리스트를 프린트함
    for(int i = 0; i < v.count(); i++)
    {
        QString list;
        for(uint j = 0; j < v.count(); j++)
        {
            list += (QString::number(unit_ping_list[i][j]) + " ");
        }
        qDebug()<<"[" + QString::number(i) + "] = " + list;
    }

    //유닛 딜레이 리스트 등록
    for(QSharedPointer<Cuma_Main> it: v)
    {
        it->mf_set_dealy_lst(unit_ping_list);
    }

    return 0;
}

void Cuma_client_test::env_clear_env()
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

    //해당 디렉토리를 삭제하고 다시 재생성
    QDir::setCurrent(root_path);

    QDir t_dir;
    t_dir.remove("test_dir");
    t_dir.mkdir("test_dir");
    QDir::setCurrent(root_path + "/test_dir");

}

int Cuma_client_test::env_make_file(QString f_name)
{
    QFile f(QDir::currentPath() + "/" + f_name);

    //만약 파일이 존재한다면 0을 리턴함
    if(f.exists() == true)
    {
        return 0;
    }

    //파일을 쓰기모드로 오픈함
    if( f.open(QFile::WriteOnly) < 0)
    {
        Cuma_Error(f.errorString(), __LINE__).show_error_string();
        return -1;
    }

    //파일의 바이너리를 저장
    for(int i = 0; i<1000; i++)
        f.write(QString::number(LONG_MAX).toUtf8());

    //파일을 닫음
    f.close();

    return 0;

}

void Cuma_client_test::env_remove_file(QString f_name)
{
    QDir dir;

    //파일 이름으로 존재하는지 확
    if(dir.exists(f_name) == true)
    {
        //파일을 삭제함
        dir.remove(f_name);
    }
}

QByteArray Cuma_client_test::env_get_file_binary(QString f_name)
{
    QFile f (QDir::currentPath() + "/" + f_name);
    QByteArray ret_binary;

    if(f.open(QFile::ReadOnly) == false)
        return nullptr;

    ret_binary = f.readAll();

    f.close();

    return ret_binary;
}

QVector<QByteArray> Cuma_client_test::env_get_file_frag(QString f_name, uint32_t count)
{
    QVector<QByteArray> f_frag_list;

    QFile f (QDir::currentPath() + "/" + f_name);
    if(f.open(QFile::ReadOnly) == false)
        return f_frag_list;

    QByteArray f_binary =  f.readAll();

    uint32_t f_frag_size = (f_binary.size()/ count);

    if(f_binary.size() % count)
        count += 1;

    for(uint i = 0; i < count; i++)
    {
        f_frag_list.append(f_binary.mid(i, f_frag_size));
    }

    return f_frag_list;
}

QVector<QSharedPointer<Cuma_Main> > Cuma_client_test::env_make_unit(uint32_t count)
{
    QVector<QSharedPointer<Cuma_Main> > ret_unit_vec;

    for(uint i = 0; i < count; i++)
    {
        ret_unit_vec.append(QSharedPointer<Cuma_Main>::create());
    }

    return ret_unit_vec;
}

QVector<uint32_t> Cuma_client_test::env_allow_ping_unit(uint32_t limit, QVector<uint32_t> delay_list, uint32_t pid)
{
    QVector<uint32_t> ret_lst;
    for(int i = 0 ; i < delay_list.count(); i++)
    {
        if(i == pid)
        {
            continue;
        }

        //만약 delay_list[i]가 limit보다 크지 않다면 리턴 리스트에 append
        if(! (delay_list[i] > limit) )
        {
            ret_lst.append(i);
        }
    }
    return ret_lst;
}

bool Cuma_client_test::env_find_pid(uint32_t i, QVector<QSharedPointer<Cuma_Main> > &v)
{
    for(QSharedPointer<Cuma_Main> it : v)
    {
        if(it->mf_get_pid() == i)
        {
            return true;
        }
    }
    return false;
}

bool Cuma_client_test::env_compare_file_frag(QVector<QByteArray> test_unit_frag_list, QVector<QByteArray> frag_list)
{
    for(uint i = 0; i < test_unit_frag_list.count(); i++)
    {
        for(uint j = 0; j< frag_list.count(); j++)
        {
            if(test_unit_frag_list[i] == frag_list[j])
            {
                return true;
            }
        }
    }

    return false;
}
