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

void Cuma_client_test::t_mf_command_set_file_name()
{
    //환경을 clear
    env_clear_cache();

    //파일 이름커맨드를 emit함
    QSignalSpy s_comm_file(this, SIGNAL(s_start_command(QJsonObject)));
    QJsonObject c_proto;

    c_proto["command_set_file_name"] = "test.txt";
    emit s_start_command(c_proto);

    Cuma_Debug("check signal emit");
    QVERIFY(s_comm_file.count() > 0);

    Cuma_Debug("check command result");
    QVERIFY(m_File->get_File_Name() == "test.txt");

}

void Cuma_client_test::t_mf_command_set_unit_bypass_count()
{
    //환경을 clear
    env_clear_cache();

    //바이패스 카운트를 세팅
    QSignalSpy s_comm_file(this, SIGNAL(s_start_command(QJsonObject)));
    QJsonObject c_proto;

    c_proto["command_set_unit_bypass_count"] = 10;
    emit s_start_command(c_proto);

    Cuma_Debug("check signal emit");
    QVERIFY(s_comm_file.count() > 0);

    Cuma_Debug("check command result");
    QVERIFY(m_limit_bypass_count == 10);
}

void Cuma_client_test::t_mf_command_ping_test()
{

    //환경을 clear
    Cuma_Debug("make test env");
    env_clear_cache();

    //ping 테스트 command 시그널을 detact할 signalspy를 만듬
    Cuma_Debug("set signalspy to command");
    QSignalSpy comm_spy(this, SIGNAL(s_start_command(QJsonObject)));

    //ping테스트를 여기로 전송할 유닛을 생성함
    Cuma_Debug("set test unit");
    QSharedPointer<Cuma_Main> test_unit = QSharedPointer<Cuma_Main>(new Cuma_Main);

    //여기로 전송을 보낼 pid를 세팅함
    Cuma_Debug("set pid test unit");
    test_unit->mf_set_pid(1);

    //현재 유닛의 pid를 세팅함
    Cuma_Debug("set pid current unit");
    mf_set_pid(0);

    //유닛의 ping의 리미트를 설정함
    Cuma_Debug("set pid test unit");
    another_pid_ping_array arr;

    //유닛의 ping의 딜레이 타이밍을 만듬
    Cuma_Debug("set ping delay timing list");
    for(int i = 0; i<2; i++)
    {
        for(int j = 0; j<2; j++)
        {
            if(i == j)
            {
                arr[i].append(0);
                continue;
            }
            else
            {
                arr[i].append(qrand() % 1000);
            }
        }
    }

    //모든 테스트 유닛의 QSharedpointer<Cuma_Main>을 세팅
    Cuma_Debug("make unit pointer array");
    QVector<QSharedPointer<Cuma_Main>> ptr_arr;

    ptr_arr.append(QSharedPointer<Cuma_Main>(this));
    ptr_arr.append(test_unit);


    //딜레이 타이밍을 각 유닛에게 세팅함
    Cuma_Debug("set delay timing to unit");
    test_unit->mf_set_dealy_lst(arr);
    this     ->mf_set_dealy_lst(arr);

    //테스트 유닛의 객체 포인터를 세팅함
    Cuma_Debug("set test unit obj pointer");
    test_unit->mf_set_unit_list(ptr_arr);
    this     ->mf_set_unit_list(ptr_arr);

    //프로토콜을 만듬
    QJsonObject obj;
    obj["command_ping_test"] = true;

    //command 프로토콜을 만듬
    Cuma_Debug("execute command");
    emit sl_start_command_signal(obj);

    //시그널이 정상적으로 emit 됬는지 확인
    Cuma_Debug("check signal is emit");
    QVERIFY (comm_spy.count() > 0);

    //유닛들의 ping 리턴값이 정상적으로 되었는지확인
    Cuma_Debug("check signal is emit");
    QVERIFY (m_Send_Unit_list.count() > 0);

    //유닛들의 ping 리턴한 유닛이 test_unit와 같은지 확인
    Cuma_Debug("return ping unit is same as test_unit");
    QVERIFY (test_unit == test_unit);
}

void Cuma_client_test::t_mf_command_spread_test()
{
    //환경을 clear
    Cuma_Debug("make test env");
    env_clear_cache();

    //파일을읽어서 m_File 의 바이너리 캐시를 만듬
    Cuma_Debug("check env make_file");
    QVERIFY (env_make_file("test.txt") > 0);

    //ping 테스트 command 시그널을 detact할 signalspy를 만듬
    Cuma_Debug("set delay timing to unit");
    QSignalSpy comm_spy(this, SIGNAL(s_start_command(QJsonObject)));

    //ping테스트를 여기로 전송할 유닛을 생성함
    Cuma_Debug("set delay timing to unit");
    QSharedPointer<Cuma_Main> test_unit = QSharedPointer<Cuma_Main>(new Cuma_Main);

    //여기로 전송을 보낼 pid를 세팅함
    Cuma_Debug("set pid to unit env");
    test_unit->mf_set_pid(1);

    //현재 유닛의 pid를 세팅함
    Cuma_Debug("set current unit pid");
    mf_set_pid(0);

    //유닛의 ping의 리미트를 설정함
    Cuma_Debug("set unit ping limit");
    another_pid_ping_array arr;

    //유닛의 ping의 딜레이 타이밍을 만듬
    Cuma_Debug("set unit ping delay timing");
    for(int i = 0; i<2; i++)
    {
        for(int j = 0; j<2; j++)
        {
            if(i == j)
            {
                arr[i].append(0);
                continue;
            }
            else
            {
                arr[i].append(qrand() % 1000);
            }
        }
    }

    //모든 테스트 유닛의 QSharedpointer<Cuma_Main>을 세팅
    Cuma_Debug("set test unit array");
    QVector<QSharedPointer<Cuma_Main>> ptr_arr;

    ptr_arr.append(QSharedPointer<Cuma_Main>(this));
    ptr_arr.append(test_unit);

    //딜레이 타이밍을 각 유닛에게 세팅함
    Cuma_Debug("set delay timing to unit");
    test_unit->mf_set_dealy_lst(arr);
    this     ->mf_set_dealy_lst(arr);

    //테스트 유닛의 객체 포인터를 세팅함
    Cuma_Debug("set test unit obj pointer");
    test_unit->mf_set_unit_list(ptr_arr);
    this     ->mf_set_unit_list(ptr_arr);

    //프로토콜을 만듬
    Cuma_Debug("make protocol");
    QJsonObject obj;

    //파일을 읽음
    Cuma_Debug("read file");
    m_File->set_File_Name("test.txt");
    m_File->read_file();

    //파일이 읽혀졌는지 확인
    Cuma_Debug("check binary is read");
    QVERIFY (m_File->get_File_binary().isEmpty() == false);

    //커맨드 실행함
    Cuma_Debug("execute command");
    obj["command_spread_test"] = true;
    emit s_start_command(obj);

    //시그널이 정상적으로 emit이 되었는지 확인
    Cuma_Debug("check signal is emit");
    QVERIFY (comm_spy.count() > 0);

    //상대방의 프로토콜 응답이 있는지 확인함
    Cuma_Debug("check signal is emit");
    QVERIFY (test_unit->mf_get_report_json()["send"].isNull() == false);

    //reply ping 프로토콜이 존재하는지 확인
    Cuma_Debug("check reply ping protocol is exsist");
    QVERIFY (m_Send_Unit_list.count() > 0);
}

void Cuma_client_test::t_mf_command_req_file_test()
{
    //파일 의 바이너리 정보를 req함

    //환경을 clear
    Cuma_Debug("make test env");
    env_clear_cache();

    //파일을읽어서 m_File 의 바이너리 캐시를 만듬
    Cuma_Debug("check env make_file");
    QVERIFY (env_make_file("test.txt") > 0);

    //ping 테스트 command 시그널을 detact할 signalspy를 만듬
    Cuma_Debug("set delay timing to unit");
    QSignalSpy comm_spy(this, SIGNAL(s_start_command(QJsonObject)));

    //ping테스트를 여기로 전송할 유닛을 생성함
    Cuma_Debug("set delay timing to unit");
    QSharedPointer<Test_Unit> test_unit = QSharedPointer<Test_Unit>(new Test_Unit);

    //여기로 전송을 보낼 pid를 세팅함
    Cuma_Debug("set pid to unit env");
    test_unit->mf_set_pid(1);

    //현재 유닛의 pid를 세팅함
    Cuma_Debug("set current unit pid");
    mf_set_pid(0);

    //유닛의 ping의 리미트를 설정함
    Cuma_Debug("set unit ping limit");
    another_pid_ping_array arr;

    //유닛의 ping의 딜레이 타이밍을 만듬
    Cuma_Debug("set unit ping delay timing");
    for(int i = 0; i<2; i++)
    {
        for(int j = 0; j<2; j++)
        {
            if(i == j)
            {
                arr[i].append(0);
                continue;
            }
            else
            {
                arr[i].append(qrand() % 1000);
            }
        }
    }

    //모든 테스트 유닛의 QSharedpointer<Cuma_Main>을 세팅
    Cuma_Debug("set test unit array");
    QVector<QSharedPointer<Cuma_Main>> ptr_arr;

    ptr_arr.append(QSharedPointer<Cuma_Main>(this));
    ptr_arr.append(test_unit);

    //딜레이 타이밍을 각 유닛에게 세팅함
    Cuma_Debug("set delay timing to unit");
    test_unit->mf_set_dealy_lst(arr);
    this     ->mf_set_dealy_lst(arr);

    //테스트 유닛의 객체 포인터를 세팅함
    Cuma_Debug("set test unit obj pointer");
    test_unit->mf_set_unit_list(ptr_arr);
    this     ->mf_set_unit_list(ptr_arr);

    //테스트 유닛이 파일을 읽음
    test_unit->read_file("test.txt");

    //테스트 유닛이 파일을 정상적으로 읽었는지 확인
    QVERIFY (test_unit->get_file_obj().get_File_binary().size() > 0);

    //파일 리퀘스트 프로토콜을 만듬
    Cuma_Debug("make protocol");
    QJsonObject obj;
    obj["command_rq_file"] = true;

    //커맨드 실행함
    Cuma_Debug("execute command");
    emit s_start_command(obj);

    //시그널이 정상적으로 emit이 되었는지 확인
    Cuma_Debug("check signal is emit");
    QVERIFY (comm_spy.count() > 0);

    //상대방의 프로토콜 응답이 있는지 확인함
    Cuma_Debug("check signal is emit");
    QVERIFY (test_unit->mf_get_report_json()["send"].isNull() == false);

    //reply ping 프로토콜이 존재하는지 확인
    Cuma_Debug("check reply ping protocol is exsist");
    QVERIFY (m_Send_Unit_list.count() > 0);

}

void Cuma_client_test::t_mf_command_trace_pass_test()
{
    //파일의 모든 유닛들에게 파일이 있는지 trace 체크

    //파일 의 바이너리 정보를 req함

    //환경을 clear
    Cuma_Debug("make test env");
    env_clear_cache();

    //파일을읽어서 m_File 의 바이너리 캐시를 만듬
    Cuma_Debug("check env make_file");
    QVERIFY (env_make_file("test.txt") > 0);

    //ping 테스트 command 시그널을 detact할 signalspy를 만듬
    Cuma_Debug("set delay timing to unit");
    QSignalSpy comm_spy(this, SIGNAL(s_start_command(QJsonObject)));

    //ping테스트를 여기로 전송할 유닛을 생성함
    Cuma_Debug("set delay timing to unit");
    QSharedPointer<Test_Unit> test_unit = QSharedPointer<Test_Unit>(new Test_Unit);

    //여기로 전송을 보낼 pid를 세팅함
    Cuma_Debug("set pid to unit env");
    test_unit->mf_set_pid(1);

    //현재 유닛의 pid를 세팅함
    Cuma_Debug("set current unit pid");
    mf_set_pid(0);

    //유닛의 ping의 리미트를 설정함
    Cuma_Debug("set unit ping limit");
    another_pid_ping_array arr;

    //유닛의 ping의 딜레이 타이밍을 만듬
    Cuma_Debug("set unit ping delay timing");
    for(int i = 0; i<2; i++)
    {
        for(int j = 0; j<2; j++)
        {
            if(i == j)
            {
                arr[i].append(0);
                continue;
            }
            else
            {
                arr[i].append(qrand() % 1000);
            }
        }
    }

    //모든 테스트 유닛의 QSharedpointer<Cuma_Main>을 세팅
    Cuma_Debug("set test unit array");
    QVector<QSharedPointer<Cuma_Main>> ptr_arr;

    ptr_arr.append(QSharedPointer<Cuma_Main>(this));
    ptr_arr.append(test_unit);

    //딜레이 타이밍을 각 유닛에게 세팅함
    Cuma_Debug("set delay timing to unit");
    test_unit->mf_set_dealy_lst(arr);
    this     ->mf_set_dealy_lst(arr);

    //테스트 유닛의 객체 포인터를 세팅함
    Cuma_Debug("set test unit obj pointer");
    test_unit->mf_set_unit_list(ptr_arr);
    this     ->mf_set_unit_list(ptr_arr);

    //테스트 유닛이 파일을 읽음
    test_unit->read_file("test.txt");

    //파일이 읽혀졌는지 확인
    Cuma_Debug("check binary is read");
    QVERIFY (test_unit->get_file_obj().get_File_binary().size() > 0);

    //테스트 유닛이 파일을 씀
    Cuma_Debug("write test unit ");
    QVERIFY(test_unit->write_file() > 0);

    //프로토콜을 만듬
    Cuma_Debug("make protocol");
    QJsonObject obj;
    obj["command_trace_pass"] = true;

    //커맨드 실행함
    Cuma_Debug("execute command");
    emit s_start_command(obj);

    //시그널이 정상적으로 emit이 되었는지 확인
    Cuma_Debug("check signal is emit");
    QVERIFY (comm_spy.count() > 0);

    //상대방의 프로토콜 응답이 있는지 확인함
    Cuma_Debug("check signal is emit");
    QVERIFY (test_unit->mf_get_report_json()["send"].isNull() == false);

    //reply ping 프로토콜이 존재하는지 확인
    Cuma_Debug("check reply ping protocol is exsist");
    QVERIFY (m_Send_Unit_list.count() > 0);
}

int Cuma_client_test::env_set_test_env_unit()
{
    return 0;
}

void Cuma_client_test::env_clear_cache()
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

Test_Unit::Test_Unit()
{

}

Test_Unit::~Test_Unit()
{

}

void Test_Unit::set_read_file_name(QString f_name)
{
    this->m_File->set_File_Name(f_name);

}

void Test_Unit::read_file()
{
    //세팅된 파일 이름을 읽음
    this->m_File->read_file_frag(this->m_File->get_File_Name(), 0);

}

void Test_Unit::read_file(QString f_name)
{
    this->m_File->set_File_Name(f_name);
    this->m_File->read_file();
}

int Test_Unit::write_file()
{
    //캐시된 파일이름와 바이너리을 이용해서 저장
    this->m_File->save_File_Frag(this->m_File->get_File_binary(), this->m_File->get_File_Name(), 0);

    return 0;
}

int Test_Unit::write_file(QString f_name, QByteArray &arr)
{
    //파일 이름을 f_name으로
    this->m_File->set_File_Name(f_name);

    //인덱스를 0으로 하고 저장
    this->m_File->save_File_Frag(arr, f_name, 0);

    return 0;
}

Cuma_File Test_Unit::get_file_obj()
{
    return *m_File;
}
