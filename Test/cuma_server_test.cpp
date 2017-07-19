#include "cuma_server_test.h"

Cuma_server_test::Cuma_server_test(QObject *parent)
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

Cuma_server_test::~Cuma_server_test()
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
void Cuma_server_test::t_f_upload_file_frag_from_unit_nofile()
{
    //테스트 환경을 flush함
    Cuma_Debug("flush test env ", __LINE__);
    env_clear_env();
    
    //테스트 유닛을 생성함
    Cuma_Debug("generate test unit ", __LINE__);
    QSharedPointer<Cuma_serv_test_unit> test_unit = QSharedPointer<Cuma_serv_test_unit>(new Cuma_serv_test_unit);
    
    //시그널 스파이
    QSignalSpy t_unit_dbg_sig(test_unit.data(), SIGNAL(s_recv(QJsonObject)));

    //env 유닛을 가지고 옴
    Cuma_Debug("set test env", __LINE__);
    env_set_env(2, QSharedPointer<Cuma_Main>(this), test_unit);
    
    //파일 체크 프로토콜을 생성함
    Cuma_Debug("generate file check protocol", __LINE__);
    QJsonObject t_protocol = cuma_protocol::req_file_binary_read_protocol("test.txt", 1, test_unit->mf_get_pid());
    
    Cuma_Debug("show send file check protcol", __LINE__);
    Cuma_Debug(QJsonDocument(t_protocol).toJson(), __LINE__);

    //각자 ping 프로토콜을 체크 함
    //프로토콜 핸들러를 실행함
    Cuma_Debug("test protocol handler", __LINE__);
    f_upload_file_frag_from_unit(t_protocol);
    
    //프로토콜 reply가 정상적으로 왔는지 체크함
    Cuma_Debug("check protocol reply is clear", __LINE__);
    QVERIFY(t_unit_dbg_sig.count() > 0);
    
    //리턴된 파라미터를 체크함
    QList<QVariant> arg = t_unit_dbg_sig.takeFirst();

    Cuma_Debug("============ recv reply test json =================");
    Cuma_Debug(QJsonDocument(arg.at(0).toJsonObject()).toJson(), __LINE__);

    QVERIFY (arg.at(0).toJsonObject()["process"] == "read");
    QVERIFY (arg.at(0).toJsonObject()["file_frag"].isNull() == false);
    QVERIFY (arg.at(0).toJsonObject()["file_name"] == "test.txt");
    QVERIFY (arg.at(0).toJsonObject()["file_index"].toInt() == 1);
    QVERIFY (arg.at(0).toJsonObject()["reply"].toBool() == true);
}

void Cuma_server_test::t_f_upload_file_frag_form_unit_success()
{
    //테스트 환경을 flush함
    Cuma_Debug("flush test env ", __LINE__);
    env_clear_env();

    //테스트 유닛을 생성함
    Cuma_Debug("generate test unit ", __LINE__);
    QSharedPointer<Cuma_serv_test_unit> test_unit = QSharedPointer<Cuma_serv_test_unit>(new Cuma_serv_test_unit);

    //시그널 스파이
    QSignalSpy t_unit_dbg_sig(test_unit.data(), SIGNAL(s_recv(QJsonObject)));

    //env 유닛을 가지고 옴
    Cuma_Debug("set test env", __LINE__);
    env_set_env(2, QSharedPointer<Cuma_Main>(this), test_unit);

    //파일frag를 만듬
    Cuma_Debug("make test file frag", __LINE__);
    QVERIFY (env_make_Frag("test.txt", 10, "Cuma_Frag_dir") == 0);

    //파일을 읽음
    Cuma_Debug("read file", __LINE__);
    QVERIFY (m_File->read_file_frag("test.txt", 1) == Cuma_File::C_F_no_err);

    //파일 체크 프로토콜을 생성함
    Cuma_Debug("generate file check protocol", __LINE__);
    QJsonObject t_protocol = cuma_protocol::req_file_binary_read_protocol("test.txt", 1, test_unit->mf_get_pid());

    Cuma_Debug("show send file check protcol", __LINE__);
    Cuma_Debug(QJsonDocument(t_protocol).toJson(), __LINE__);

    //각자 ping 프로토콜을 체크 함
    //프로토콜 핸들러를 실행함
    Cuma_Debug("test protocol handler", __LINE__);
    f_upload_file_frag_from_unit(t_protocol);

    //프로토콜 reply가 정상적으로 왔는지 체크함
    Cuma_Debug("check protocol reply is clear", __LINE__);
    QVERIFY(t_unit_dbg_sig.count() > 0);

    //리턴된 파라미터를 체크함
    QList<QVariant> arg = t_unit_dbg_sig.takeFirst();

    Cuma_Debug("============ recv reply test json =================");
    Cuma_Debug(QJsonDocument(arg.at(0).toJsonObject()).toJson(), __LINE__);

    QVERIFY (arg.at(0).toJsonObject()["process"] == "read");
    QVERIFY (arg.at(0).toJsonObject()["file_frag"].isNull() == false);
    QVERIFY (arg.at(0).toJsonObject()["file_name"] == "test.txt");
    QVERIFY (arg.at(0).toJsonObject()["file_index"].toInt() == 1);
    QVERIFY (arg.at(0).toJsonObject()["reply"].toBool() == true);
    QVERIFY (arg.at(0).toJsonObject()["Error"].isNull() == true);
}
void Cuma_server_test::t_f_reply_upload_file_frag_to_unit()
{
    //테스트 환경을 flush함
    Cuma_Debug("flush test env", __LINE__);
    env_clear_env();
    
    //테스트 유닛을 생성함
    Cuma_Debug("generate test unit ", __LINE__);
    QSharedPointer<Cuma_serv_test_unit> test_unit = QSharedPointer<Cuma_serv_test_unit>(new Cuma_serv_test_unit);
    
    //env 유닛을 가지고 옴
    Cuma_Debug("set test env", __LINE__);
    env_set_env(2, QSharedPointer<Cuma_Main>(this), test_unit);
    
    //파일 reply 프로토콜을 만듬
    QByteArray arr = env_get_file_binary("test.txt" + QString::number(0));

    //시그널 스파이
    QSignalSpy t_unit_spy(test_unit.data(), SIGNAL(s_recv(QJsonObject)));

    //reply 프로토콜이 nullptr이 아닌지 확인
    QVERIFY(arr != nullptr);

    Cuma_Debug("generate file check protocol", __LINE__);
    QJsonObject t_protocol = cuma_protocol::reply_file_binary_read_protocol(1, "test.txt", 0, arr);
    
    //f_reply_upload_file를 실행함
    f_reply_upload_file_frag_to_unit(t_protocol);

    //파일이 저장되었는지 체크
    QVERIFY (m_File->read_file_frag("test.txt", 0) == Cuma_File::C_F_no_err);
}

//------------ 여기부터 테스트 파일 재구성
void Cuma_server_test::t_f_download_file_frag_from_unit()
{
    //테스트 환경을 flush함
    Cuma_Debug("flush test env", __LINE__);
    env_clear_env();
    
    //테스트 유닛을 생성함
    Cuma_Debug("generate test unit ", __LINE__);
    QSharedPointer<Cuma_serv_test_unit> test_unit = QSharedPointer<Cuma_serv_test_unit>(new Cuma_serv_test_unit);
    
    //env 유닛을 가지고 옴
    Cuma_Debug("set test env", __LINE__);
    env_set_env(2, QSharedPointer<Cuma_Main>(this), test_unit);
    
    QSignalSpy test_unit_spy(test_unit.data(), SIGNAL(s_recv(QJsonObject)));

    //파일 frag를 읽음
    Cuma_Debug("read file frag", __LINE__);
    QVERIFY (m_File->read_file_frag("test.txt", 1) == Cuma_File::C_F_no_err);
    
    //t_binary를 읽음
    Cuma_Debug("read t_binary", __LINE__);
    QByteArray t_binary = m_File->get_File_binary();
    
    //m_File의 테스트를 전부 clear
    Cuma_Debug("clear m_File test", __LINE__);
    m_File->clear_binary();
    
    //파일 바이너리 저장 프로토콜을 작성
    Cuma_Debug("save file binary protocol", __LINE__);
    QJsonObject t_protocol = cuma_protocol::req_file_binary_save_protocol("test.txt", 1, t_binary, test_unit->mf_get_pid());
    
    //프로토콜 핸들러를 실행
    Cuma_Debug("execute protocol handler", __LINE__);
    f_download_file_frag_from_unit(t_protocol);
    
    //프로토콜 reply가 정상적으로 왔는지 체크함
    Cuma_Debug("check reply is confirm", __LINE__);
    QVERIFY(test_unit_spy.count() > 0);

    QList<QVariant> arg = test_unit_spy.first();
    Cuma_Debug("============ recv reply test json =================");
    Cuma_Debug(QJsonDocument(arg.at(0).toJsonObject()).toJson());

    QVERIFY (arg.at(0).toJsonObject()["From"] == 0);
    QVERIFY (arg.at(0).toJsonObject()["process"] == "save");
    QVERIFY (arg.at(0).toJsonObject()["file_name"] == "test.txt");
    QVERIFY (arg.at(0).toJsonObject()["file_index"] == 1);
    QVERIFY (arg.at(0).toJsonObject()["reply"] == true);

}

void Cuma_server_test::t_f_reply_download_file_frag_to_unit()
{
    //테스트 환경을 flush함
    Cuma_Debug("flush test env", __LINE__);
    env_clear_env();
    
    //테스트 유닛을 생성함
    Cuma_Debug("generate test unit ", __LINE__);
    QSharedPointer<Cuma_serv_test_unit> test_unit = QSharedPointer<Cuma_serv_test_unit>(new Cuma_serv_test_unit);
    
    //env 유닛을 가지고 옴
    Cuma_Debug("set test env", __LINE__);
    env_set_env(2, QSharedPointer<Cuma_Main>(this), test_unit);
    
    //파일 frag를 읽음
    Cuma_Debug("read file frag", __LINE__);
    QVERIFY (m_File->read_file_frag("test.txt", 1) == Cuma_File::C_F_no_err);
    
    //t_binary를 읽음
    Cuma_Debug("read t_binary", __LINE__);
    QByteArray t_binary = m_File->get_File_binary();
    
    //파일 바이너리 저장 프로토콜을 작성
    Cuma_Debug("clear m_File test", __LINE__);
    QJsonObject t_protocol = cuma_protocol::reply_file_binary_save_protocol(1, "test.txt", 0);
    
    //프로토콜 핸들러를 실행함
    Cuma_Debug("execute protocol handler", __LINE__);
    f_reply_download_file_frag_to_unit(t_protocol);
}

void Cuma_server_test::t_f_check_file_frag_to_unit()
{
    //테스트 환경을 flush함
    Cuma_Debug("flush test env", __LINE__);
    env_clear_env();
    
    //테스트 유닛을 생성함
    Cuma_Debug("generate test unit ", __LINE__);
    QSharedPointer<Cuma_serv_test_unit> test_unit = QSharedPointer<Cuma_serv_test_unit>(new Cuma_serv_test_unit);
    
    //env 유닛을 가지고 옴
    Cuma_Debug("set test env", __LINE__);
    env_set_env(2, QSharedPointer<Cuma_Main>(this), test_unit);
    
    //캡처할 유닛 spy
    QSignalSpy test_unit_spy(test_unit.data(), SIGNAL(s_recv(QJsonObject)));

    //파일 frag를 읽음
    Cuma_Debug("read file frag", __LINE__);
    QVERIFY (m_File->read_file_frag("test.txt", 1) == Cuma_File::C_F_no_err);
    
    //파일 바이너리 체크 프로토콜을 작성
    Cuma_Debug("clear m_File test", __LINE__);
    QJsonObject t_protocol = cuma_protocol::req_is_file_exsist_protocol("test.txt", 1, test_unit->mf_get_pid(), true);

    //프로토콜 핸들러를 실행함
    Cuma_Debug("execute protocol handler", __LINE__);
    f_check_file_frag_to_unit(t_protocol);
    
    QVERIFY (test_unit_spy.count() > 0);

    Cuma_Debug("convert signal argument to json", __LINE__);
    QList<QVariant> arg = test_unit_spy.first();

    Cuma_Debug("============ recv reply test json =================");
    Cuma_Debug(QJsonDocument(arg.at(0).toJsonObject()).toJson());

    QVERIFY (arg.at(0).toJsonObject()["From"].toInt() == m_Pid);
    QVERIFY (arg.at(0).toJsonObject()["process"].toString() == "check_file");
    QVERIFY (arg.at(0).toJsonObject()["file_name"] == "test.txt");
    QVERIFY (arg.at(0).toJsonObject()["file_index"] == 1);
    QVERIFY (arg.at(0).toJsonObject()["reply"] == true);
}

void Cuma_server_test::t_f_reply_check_file_frag_to_unit()
{
    //테스트 환경을 flush함
    Cuma_Debug("flush test env", __LINE__);
    env_clear_env();
    
    //테스트 유닛을 생성함
    Cuma_Debug("generate test unit ", __LINE__);
    QSharedPointer<Cuma_serv_test_unit> test_unit = QSharedPointer<Cuma_serv_test_unit>(new Cuma_serv_test_unit);
    
    //env 유닛을 가지고 옴
    Cuma_Debug("set test env", __LINE__);
    env_set_env(2, QSharedPointer<Cuma_Main>(this), test_unit);
    
    //파일 체크 프로토콜을 생성함
    Cuma_Debug("read file frag", __LINE__);
    QJsonObject t_protocol = cuma_protocol::reply_is_file_exsist_protocol("test.txt", 1, 1, true);

    //프로토콜 핸들러를 실행함
    Cuma_Debug("reply check  protocol handler", __LINE__);
    f_reply_check_file_frag_to_unit(t_protocol);
    
    //해당 pid에 인덱스가 있는지 확인
    QVERIFY (m_file_frag_address["test.txt"].isEmpty() == false);


}

void Cuma_server_test::t_f_over_bypass()
{
    QJsonObject t_protocol;

    //테스트 환경을 flush함
    Cuma_Debug("flush test env", __LINE__);
    env_clear_env();
    
    //테스트 유닛을 생성함
    Cuma_Debug("generate test unit ", __LINE__);
    QSharedPointer<Cuma_serv_test_unit> test_unit_0 = QSharedPointer<Cuma_serv_test_unit>::create();
    
    //env 유닛을 가지고 옴
    Cuma_Debug("set test env", __LINE__);
    env_set_env(2, QSharedPointer<Cuma_Main>(this), test_unit_0);
    
    //test 유닛의 signalspy
    QSignalSpy test_unit_spy(test_unit_0.data(), SIGNAL(s_recv(QJsonObject)));

    //기존의 유닛 프로토콜을 넣음
    t_protocol = cuma_protocol::req_is_file_exsist_protocol("test.txt", 0, 0, true);

    //바이패스 리미트 카운트를 1로 넣음
    t_protocol["bypass_limit_count"] = 1;

    Cuma_Debug("============ recv reply test json =================");
    Cuma_Debug(QJsonDocument(t_protocol).toJson());

    f_over_bypass(t_protocol);

    //바이패스 프로토콜이 왔는지 확인
    QVERIFY (test_unit_spy.count() > 0);

    Cuma_Debug("convert signal argument to json", __LINE__);
    QList<QVariant> arg = test_unit_spy.first();

    Cuma_Debug("============ recv reply test json =================");
    Cuma_Debug(QJsonDocument(arg.at(0).toJsonObject()).toJson());

    QVERIFY (arg.at(0).toJsonObject()["From"].toInt() == m_Pid);
    QVERIFY (arg.at(0).toJsonObject()["bypass"].toArray().isEmpty() == false);
    QVERIFY (arg.at(0).toJsonObject()["bpass_count"].toInt() == 0);
}

void Cuma_server_test::t_f_reply_over_bypass_limit()
{
    //테스트 환경을 flush함
    env_clear_env();
    
    //테스트 유닛을 생성함
    QSharedPointer<Cuma_serv_test_unit> test_unit_0 = QSharedPointer<Cuma_serv_test_unit>::create();
    
    //env 유닛을 가지고 옴
    env_set_env(2, QSharedPointer<Cuma_Main>(this), test_unit_0);

    QSignalSpy test_unit_spy(test_unit_0.data(), SIGNAL(s_recv(QJsonObject)));



    //프로토콜을 만듬
    QJsonObject protocol;
    protocol["bypass_count"] = 1;

    QJsonArray bypass_arr = protocol["bypass"].toArray();
    bypass_arr.append(static_cast<int>(test_unit_0->mf_get_pid()));
    bypass_arr.append(static_cast<int>(m_Pid));
    protocol["bypass"] = bypass_arr;

    Cuma_Debug("============ recv reply test json =================");
    Cuma_Debug(QJsonDocument(protocol).toJson());

    f_reply_over_bypass_limit(protocol);

    //수신 시그널이 emit이 되었는지 확인
    QVERIFY(test_unit_spy.count() > 0);

    Cuma_Debug("============ signal paramter json =================");
    QList<QVariant> arg = test_unit_spy.first();
    Cuma_Debug(QJsonDocument(arg.at(0).toJsonObject()).toJson());



    //프로토콜이 0인 경우
    protocol["bypass_count"] = 0;

    QJsonArray bypass_arr_2;
    bypass_arr_2.append(static_cast<int>(m_Pid));
    protocol["bypass"] = bypass_arr_2;

    Cuma_Debug("============ recv reply test json =================");
    Cuma_Debug(QJsonDocument(protocol).toJson());

    f_reply_over_bypass_limit(protocol);

    QVERIFY(m_bypass_protocol_queue.count() > 0);
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

int Cuma_server_test:: env_make_Frag(QString f_name, uint32_t count)
{
    Cuma_Debug("env_make_Frag path : " + QDir::currentPath());
    for(int i = 0; i < count; i++)
    {
        QFile f(QDir::currentPath() + "/" + f_name + QString::number(i));
        
        //만약 파일이 존재한다면 0을 리턴함
        if(f.exists() == true)
        {
            continue;
        }
        
        //파일을 쓰기모드로 오픈함
        if( f.open(QFile::WriteOnly) != true)
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

int Cuma_server_test::env_make_File(QString f_name, QString path)
{
    QDir dir;

    if (dir.exists(path) == false)
    {
        dir.mkdir(path);
    }

    dir.cd(path);

    QFile f(dir.absolutePath() + "/" + f_name);

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

int Cuma_server_test::env_make_Frag(QString f_name, uint32_t count, QString path)
{
    QDir dir;

    if (dir.exists(path) == false)
    {
        dir.mkdir(path);
    }

    dir.cd(path);

    Cuma_Debug("env_make_Frag path : " + dir.absolutePath());
    for(int i = 0; i < count; i++)
    {
        QFile f(dir.absolutePath() + "/" + f_name + QString::number(i));

        //만약 파일이 존재한다면 0을 리턴함
        if(f.exists() == true)
        {
            continue;
        }

        //파일을 쓰기모드로 오픈함
        if( f.open(QFile::WriteOnly) != true)
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

    QStringList dir_entry = dir.entryList();

    Cuma_Debug(" test file Entry list ");
    for(QString e: dir_entry)
    {
        Cuma_Debug(e, __LINE__);
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
        return NULL;
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
    try
    {

        //유닛들을 clear함
        for(QSharedPointer<Cuma_Main>& m : unit_vec)
        {
            m->get_File_obj()->clear_binary();
        }

        //pid를 세팅
        uint i = 0;
        for(QSharedPointer<Cuma_Main>& m : unit_vec)
        {
            m->mf_set_pid(i);
            i++;
        }

        another_pid_ping_array t_unit_arr;

        for(int i = 0; i < unit_vec.size() ; i++)
        {
            my_ping_list add_list;
            add_list.resize(unit_vec.size());
            t_unit_arr.append(add_list);
        }

        for(int i = 0; i < unit_vec.size() ; i++)
        {
            for(int j = 0; j < unit_vec.size() ; j ++)
            {
                if(i == j)
                {
                    t_unit_arr[i].replace(j, 0);
                }
                else if(i < j)
                {
                    uint32_t rand_num = (qrand() % 100);

                    //[i][j]에 넣기
                    t_unit_arr[i].replace(j, rand_num);

                    //[j][i]에 넣기
                    t_unit_arr[j].replace(i, rand_num);
                }
            }
        }

        //유닛 딜레이 리스트 & 유닛 리티트 유닛 포인터 등록
        for(int i = 0; i < unit_vec.size() ; i++)
        {
            unit_vec[i]->mf_set_dealy_lst(t_unit_arr);
            unit_vec[i]->mf_t_set_limit_unit(unit_vec);
        }

        /*//init 유닛의 테스트 디렉토리를 test_dir + uid으로 생성함
    for(uint i = 0; i< count; i++)
    {
        QDir dir;
        QString cur_dir_path = dir.currentPath();

        QString test_unit_dir_name = "test_dir" + QString::number(unit_vec[i]->mf_get_pid());

        if(!dir.exists(test_unit_dir_name))
        {
            dir.mkdir(test_unit_dir_name);
        }

        dir.cd(test_unit_dir_name);

        QDir::setCurrent(dir.currentPath());

        //해당 유닛이 m_File를 init함
        unit_vec[i]->m_File->init_dir();

        QDir::setCurrent(cur_dir_path);

    }*/
    }catch(QException& e)
    {
        Cuma_Error(e.what(), __LINE__).show_error_string();
    }

}

void Cuma_server_test::env_set_env(uint32_t count, QSharedPointer<Cuma_Main> a, QSharedPointer<Cuma_Main> b)
{
    QVector<QSharedPointer<Cuma_Main>> t_unit_arg_list;
    t_unit_arg_list.append(a);
    t_unit_arg_list.append(b);

    Cuma_Debug("init unit", __LINE__);
    init_unit(count, t_unit_arg_list);

    Cuma_Debug("make file", __LINE__);
    env_make_File("test.txt");

    Cuma_Debug("make file frag", __LINE__);
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
