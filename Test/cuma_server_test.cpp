#include "cuma_server_test.h"

Cuma_server_test::Cuma_server_test(QObject *parent)
{
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

Cuma_server_test::~Cuma_server_test()
{
    //테스트 디렉토리 삭제
}
void Cuma_server_test::t_f_upload_file_frag_from_unit()
{
    //테스트 환경을 flush함
    env_clear_env();

    //테스트 유닛을 생성함
    QSharedPointer<Cuma_serv_test_unit> test_unit = QSharedPointer<Cuma_serv_test_unit>(new Cuma_serv_test_unit);

    //env 유닛을 가지고 옴
    env_set_env(2, this, test_unit);

    //파일 체크 프로토콜을 생성함
    QJsonObject t_protocol = cuma_protocol::req_file_binary_read_protocol("test.txt", 1, 1);

    //프로토콜 핸들러를 실행함
    f_upload_file_frag_from_unit(t_protocol);

    //프로토콜 reply가 정상적으로 왔는지 체크함
    QVERIFY(test_unit->mf_get_report_json()["recv"].toArray().count() > 0);

    //해당 env에 파일 바이너리가 있는지 확인
    QVERIFY(m_File->get_File_Name() == "test.txt" > 0 || m_File->get_File_Index() == 1);
}
void Cuma_server_test::t_f_reply_upload_file_frag_to_unit()
{
    //테스트 환경을 flush함
    env_clear_env();

    //테스트 유닛을 생성함
    QSharedPointer<Cuma_serv_test_unit> test_unit = QSharedPointer<Cuma_serv_test_unit>(new Cuma_serv_test_unit);

    //env 유닛을 가지고 옴
    env_set_env(2, this, test_unit);

    //파일 체크 프로토콜을 생성함
    QJsonObject t_protocol = cuma_protocol::req_file_binary_read_protocol("test.txt", 1, 0);

    //프로토콜 핸들러를 실행함
    emit test_unit->s_recv(t_protocol);

    //프로토콜 reply가 정상적으로 왔는지 체크함
    QVERIFY(mf_get_report_json()["recv"].toArray().count() > 0);

    //해당 env에 파일 바이너리가 있는지 확인
    QVERIFY(m_File->get_File_Name() == "test.txt" > 0 || m_File->get_File_Index() == 1);
}

void Cuma_server_test::t_f_download_file_frag_from_unit()
{
    //테스트 환경을 flush함
    env_clear_env();

    //테스트 유닛을 생성함
    QSharedPointer<Cuma_serv_test_unit> test_unit = QSharedPointer<Cuma_serv_test_unit>(new Cuma_serv_test_unit);

    //env 유닛을 가지고 옴
    env_set_env(2, this, test_unit);

    //파일 frag를 읽음
    QVERIFY (m_File->read_file_frag("test.txt", 1) > 0);

    //t_binary를 읽음
    QByteArray t_binary = m_File->get_File_binary();

    //m_File의 테스트를 전부 clear
    m_File->clear_binary();

    //파일 바이너리 저장 프로토콜을 작성
    QJsonObject t_protocol = cuma_protocol::req_file_binary_save_protocol("test.txt", 1, t_binary, test_unit->mf_get_pid());

    //프로토콜 핸들러를 실행
    s_recv(t_protocol);

    //프로토콜 reply가 정상적으로 왔는지 체크함
    QVERIFY(test_unit->mf_get_report_json()["recv"].toArray().count() > 0);
}

void Cuma_server_test::t_f_reply_download_file_frag_to_unit()
{
    //테스트 환경을 flush함
    env_clear_env();

    //테스트 유닛을 생성함
    QSharedPointer<Cuma_serv_test_unit> test_unit = QSharedPointer<Cuma_serv_test_unit>(new Cuma_serv_test_unit);

    //env 유닛을 가지고 옴
    env_set_env(2, this, test_unit);

    //파일 frag를 읽음
    QVERIFY (m_File->read_file_frag("test.txt", 1) > 0);

    //t_binary를 읽음
    QByteArray t_binary = m_File->get_File_binary();

    //파일 바이너리 저장 프로토콜을 작성
    QJsonObject t_protocol = cuma_protocol::req_file_binary_save_protocol("test.txt", 1, t_binary, test_unit->mf_get_pid());

    //테스트 유닛에게 프로토콜 핸들러를 실행함
    test_unit->s_recv(t_protocol);

    //프로토콜 reply가 정상적으로 왔는지 체크함
    QVERIFY(mf_get_report_json()["recv"].toArray().count() > 0);
}

void Cuma_server_test::t_f_check_file_frag_to_unit()
{
    //테스트 환경을 flush함
    env_clear_env();

    //테스트 유닛을 생성함
    QSharedPointer<Cuma_serv_test_unit> test_unit = QSharedPointer<Cuma_serv_test_unit>(new Cuma_serv_test_unit);

    //env 유닛을 가지고 옴
    env_set_env(2, this, test_unit);

    //파일 frag를 읽음
    QVERIFY (m_File->read_file_frag("test.txt", 1) > 0);

    //t_binary를 읽음
    QByteArray t_binary = m_File->get_File_binary();

    //바이너리 clear
    m_File->clear_binary();

    //파일 바이너리 체크 프로토콜을 작성
    QJsonObject t_protocol = cuma_protocol::req_is_file_exsist_protocol("test.txt", 1);

    //테스트 유닛이 프로토콜 핸들러를 실행함
    emit test_unit->s_recv(t_protocol);

    //테스트 프로토콜의 핸들러가 정상적으로 작동됬는지 확인
    QVERIFY(test_unit->mf_get_report_json()["recv"].toArray().count() > 0);
}

void Cuma_server_test::t_f_reply_check_file_frag_to_unit()
{
    //테스트 환경을 flush함
    env_clear_env();

    //테스트 유닛을 생성함
    QSharedPointer<Cuma_serv_test_unit> test_unit = QSharedPointer<Cuma_serv_test_unit>(new Cuma_serv_test_unit);

    //env 유닛을 가지고 옴
    env_set_env(2, this, test_unit);

    //파일 체크 프로토콜을 생성함
    QJsonObject t_protocol = cuma_protocol::req_is_file_exsist_protocol("test.txt", 1);

    //프로토콜 핸들러를 실행함
    emit s_recv(t_protocol);

    //프로토콜 reply가 정상적으로 왔는지 체크함
    QVERIFY(mf_get_report_json()["recv"].toArray().count() > 0);

    //수신된 m_File_Frag_Address가 있는지 확인
    QVERIFY(m_file_frag_address.count() > 0);
}

void Cuma_server_test::t_f_over_bypass()
{
    //테스트 환경을 flush함
    env_clear_env();

    //테스트 유닛을 생성함
    QSharedPointer<Cuma_serv_test_unit> test_unit_0 = QSharedPointer<Cuma_serv_test_unit>::create();
    QSharedPointer<Cuma_serv_test_unit> test_unit_1 = QSharedPointer<Cuma_serv_test_unit>::create();

    //env 유닛을 가지고 옴
    env_set_env(3, test_unit_0, this, test_unit_1);

    //파일 frag를 읽음
    QVERIFY (m_File->read_file_frag("test.txt", 1) > 0);

    //t_binary를 읽음
    QByteArray t_binary = m_File->get_File_binary();

    //t_binary캐시를 clear함
    m_File->clear_binary();

    //test_unit_1에 파일 frag를 저장
    test_unit_1->write_file_frag("test.txt", t_binary, 1);

    //파일 search 프로토콜을 작성해서 bypass함
    QJsonObject t_protocol = cuma_protocol::req_is_file_exsist_protocol("test.txt", this->mf_get_pid());

    //테스트 유닛에게 프로토콜 핸들러를 실행함 그럼 바이패스가 시작
    test_unit_0->s_recv(t_protocol);

    //reply가 왔는지 체크
    QVERIFY(mf_get_report_json()["recv"].toArray().count() == 2);

    //수신된 m_File_Frag_Address가 있는지 확인
    QVERIFY(m_file_frag_address.count() > 0);

}

void Cuma_server_test::t_f_reply_over_bypass_limit()
{
    //테스트 환경을 flush함
    env_clear_env();

    //테스트 유닛을 생성함
    QSharedPointer<Cuma_serv_test_unit> test_unit_0 = QSharedPointer<Cuma_serv_test_unit>::create();
    QSharedPointer<Cuma_serv_test_unit> test_unit_1 = QSharedPointer<Cuma_serv_test_unit>::create();

    //env 유닛을 가지고 옴
    env_set_env(3, this, test_unit_0, test_unit_1);

    //파일 frag를 읽음
    QVERIFY (m_File->read_file_frag("test.txt", 1) > 0);

    //t_binary를 읽음
    QByteArray t_binary = m_File->get_File_binary();

    //t_binary캐시를 clear함
    m_File->clear_binary();

    //test_unit_1에 파일 frag를 저장
    test_unit_1->write_file_frag("test.txt", t_binary, 1);

    //파일 search 프로토콜을 작성해서 bypass함
    QJsonObject t_protocol = cuma_protocol::req_is_file_exsist_protocol("test.txt", 0);

    //테스트 유닛에게 프로토콜 핸들러를 실행함 그럼 바이패스가 시작
    s_recv(t_protocol);

    //reply가 왔는지 체크
    QVERIFY(mf_get_report_json()["recv"].toArray().count() == 2);

    //send가 2번인지 체크
    QVERIFY(mf_get_report_json()["send"].toArray().count() == 2);

}

int Cuma_server_test::env_make_File(QString f_name)
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

int Cuma_server_test::env_make_Frag(QString f_name, uint32_t count)
{

    for(int i = 0; i < count; i++)
    {
        QFile f(QDir::currentPath() + "/" + f_name + QString::number(count));

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
    }

    return 0;
}

QByteArray Cuma_server_test::env_get_file_binary(QString f_name)
{
    try
    {
        QFile f(QDir::currentPath() + "/" + f_name);

        //만약 파일이 존재한다면 0을 리턴함
        if(f.exists() != true)
        {
            return nullptr;
        }

        if(f.isOpen() == true)
        {
            return f.readAll();
        }

        if ( f.open(QFile::ReadOnly) == false)
        {
            throw Cuma_Error(f.errorString(), __LINE__);
        }

        return f.readAll();

    }catch(Cuma_Error& e)
    {
        e.show_error_string();
    }
}

void Cuma_server_test::env_clear_env()
{
    QDir t_dir;
    QStringList t_entry_list= t_dir.entryList();

    for(QString& e :t_entry_list)
    {
        t_dir.remove(e);
    }
}

void Cuma_server_test::env_clear_binary()
{
    m_File->clear_binary();
}

void Cuma_server_test::init_unit(uint32_t count, QVector<QSharedPointer<Cuma_Main>>& unit_vec)
{
    another_pid_ping_array t_unit_arr;

    for(uint i = 0; i < count ; i++)
    {
        for(int j = 0; j < count; j ++)
        {
            if(i == j)
            {
                t_unit_arr[i].append(0);
            }
            else
            {
                t_unit_arr[i].append(qrand() % 100);
            }
        }
    }

    for(uint i = 0; i < count ; i++)
    {
        unit_vec[i]->mf_set_dealy_lst(t_unit_arr);
        unit_vec[i]->mf_set_unit_list(unit_vec);
    }
}

void Cuma_server_test::env_set_env(uint32_t count, ...)
{
    QVector<QSharedPointer<Cuma_Main>> t_unit_arg_list;

    for(uint i = 0; i < count; i++)
    {
        t_unit_arg_list.append(va_arg(i, QSharedPointer<Cuma_Main>));
    }

    init_unit(count, t_unit_arg_list);
    env_make_File("test.txt");
    env_make_Frag("test.txt", 10);
}




Cuma_serv_test_unit::Cuma_serv_test_unit()
{

}

Cuma_serv_test_unit::~Cuma_serv_test_unit()
{

}

void Cuma_serv_test_unit::read_file()
{
    m_File->read_file();
}

void Cuma_serv_test_unit::read_file(QString f_name)
{
    m_File->set_File_Name(f_name);
    m_File->read_file();
}

void Cuma_serv_test_unit::set_file_name(QString f_name)
{
    m_File->set_File_Name(f_name);
}

void Cuma_serv_test_unit::write_file()
{
    m_File->save_File_Frag(m_File->get_File_Frag(), m_File->get_File_Name());
}

void Cuma_serv_test_unit::write_file(QString f_name, QByteArray &arr)
{
    m_File->save_File_Frag(arr, f_name, 0);
}

void Cuma_serv_test_unit::write_file_frag(QString f_name, QByteArray &arr, uint32_t index)
{
    m_File->save_File_Frag(arr, f_name, index);
}

void Cuma_serv_test_unit::env_clear_test_unit_env()
{
    m_File->clear_binary();
}
