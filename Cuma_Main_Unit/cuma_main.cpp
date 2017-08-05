#include "cuma_main.h"

uint32_t Cuma_Main::m_limit_bypass_count = 0;

bool unit_Timer::is_start = false;

QTime unit_Timer::time;

Cuma_Main::Cuma_Main(QObject *parent) : QObject(parent)
{
    Cuma_Debug::show_debug(true);

    m_File = QSharedPointer<Cuma_File>(new Cuma_File);

    //class timer가 시작을 했는지 체크
    if(unit_Timer::is_start == false)
    {
        Cuma_Debug("unit_Timer::time is start");

        //유닛의 타임을 시작함
        unit_Timer::time.start();

        unit_Timer::is_start = true;
    }

    Cuma_Debug("Binding the Signal");

    //stop 시그널을 바인딩함
    connect(this, SIGNAL(s_stop_unit()), this, SLOT(sl_stop_unit()));

    //#ifndef TEST
    //recv 시그널을 바인딩함
    connect(this, SIGNAL(s_recv(QJsonObject)), this, SLOT(sl_recv_signal(QJsonObject)));

    //s_start_spread 시그널을 바인딩함
    connect(this, SIGNAL(s_start_command(QJsonObject)), this, SLOT(sl_start_command_signal(QJsonObject)));
    /*#endif

#ifdef TEST
    connect (this, SIGNAL(s_recv(QJsonObject)), this, SLOT(sl_recv_signal(const QJsonObject)));

    //s_start_spread 시그널을 바인딩함
    connect(this, SIGNAL(s_start_command(const QJsonObject)), this, SLOT(sl_recv_test(const QJsonObject)));

#endif*/

    m_bypass_protocol_layer_active = false;
}

Cuma_Main::Cuma_Main(Cuma_Main &m)
{
    m_Unit_delay_time_array = m.m_Unit_delay_time_array;
    m_Cuma_unit_list = m.m_Cuma_unit_list;
    m_Cuma_unit_inside_timeout_unit_list = m.m_Cuma_unit_inside_timeout_unit_list;
    m_Send_Unit_list = m.m_Send_Unit_list;
    m_File = m.m_File;
    m_Pid = m.m_Pid;
    m_ping_limit = m.m_ping_limit;
    m_report_json = m.m_report_json;
    m_active = m.m_active;
    m_limit_bypass_count = m.m_limit_bypass_count;
    m_file_info_block = m.m_file_info_block;
    m_file_frag_address = m.m_file_frag_address;

}

Cuma_Main::~Cuma_Main()
{
    Cuma_Debug("Disconnect unit signal");

    //stop 시그널을 바인딩함
    disconnect(this, SIGNAL(s_stop_unit()), this, SLOT(sl_stop_unit()));

    //recv 시그널을 언바인딩함
    disconnect(this, SIGNAL(s_recv(QJsonObject)), this, SLOT(sl_recv_signal(QJsonObject)));

    //s_start_spread 시그널을 바인딩함
    disconnect(this, SIGNAL(s_start_command(const QJsonObject)), this, SLOT(sl_start_command_signal(const QJsonObject)));

}

void Cuma_Main::mf_set_unit_list(QVector<QSharedPointer<Cuma_Main>> list)
{
    Cuma_Debug("set unit list");
    if (list.isEmpty() == false)
    {
        Cuma_Debug("parameter is not empty save the unit QSharedPointer<Cuma_Main> ");
        m_Cuma_unit_list = list;
    }
}

QVector<QSharedPointer<Cuma_Main>> Cuma_Main::mf_get_unit_list()
{
    return m_Cuma_unit_list;
}

QVector<QVector<uint32_t>> Cuma_Main::mf_get_dealy_lst()
{
    return m_Unit_delay_time_array;
}

void Cuma_Main::mf_set_dealy_lst(QVector<QVector<uint32_t>> &t)
{
    m_Unit_delay_time_array = t;
}

void Cuma_Main::mf_set_ping_limit_time(uint32_t _time)
{
    m_ping_limit = _time;
}

uint32_t Cuma_Main::mf_get_ping_limit_time()
{
    return m_ping_limit;
}

void Cuma_Main::mf_set_pid(uint32_t pid)
{
    m_Pid = pid;
}

uint32_t Cuma_Main::mf_get_pid()
{
    return m_Pid;
}

QJsonObject Cuma_Main::mf_get_report_json()
{
    return m_report_json;
}

bool Cuma_Main::mf_is_active()
{
    return m_active;
}

void Cuma_Main::mf_set_active(bool b)
{
    m_active = b;
}

bool Cuma_Main::mf_get_client_bypass_protocol_layer()
{
    return m_bypass_protocol_layer_active;
}

void Cuma_Main::mf_set_client_bypass_protocol_layer(bool b)
{
    m_bypass_protocol_layer_active = b;
}

QSharedPointer<Cuma_File> Cuma_Main::get_File_obj()
{
    return m_File;
}

void Cuma_Main::mf_t_set_limit_unit(QVector<QSharedPointer<Cuma_Main> > &v)
{
    m_Cuma_unit_inside_timeout_unit_list = v;
}

QVector<QSharedPointer<Cuma_Main> > Cuma_Main::mf_t_get_unit_inside_timeout()
{
    return m_Cuma_unit_inside_timeout_unit_list;
}

void Cuma_Main::set_bypass_limit_count(uint32_t limit)
{
    m_limit_bypass_count = limit;
}

uint32_t Cuma_Main::get_bypass_limit_count()
{
    return m_limit_bypass_count;
}

void Cuma_Main::mf_set_file_frag_address(QMap<QString, QMap<uint32_t, QVector<uint32_t> > > &m)
{
    m_file_frag_address = m;
}

QMap<QString, QMap<uint32_t, QVector<uint32_t>>> Cuma_Main::mf_get_file_frag_address()
{
    return m_file_frag_address;
}

void Cuma_Main::sl_stop_unit()
{
    Cuma_Debug("stop_unit is dected : Unit ID : " + m_Pid);

    //전송 유닛리스트들이 현재 남아있는지 확인함
    if (m_Send_Unit_list.count() > 0)
    {
        //디버그 메세지
        Cuma_Debug("m_Send_Unit_list is exsist flush them all : Unit ID : " + m_Pid);

        //f_pop_unit()이 nullptr를 리턴할때까지 반복함
        while(f_pop_unit() != nullptr){}

        //반복자가 완료됨을 나타내는 디버그 메세지 표시함
        Cuma_Debug("flush m_Send_Unit_list : Unit ID : " + m_Pid);
    }

    //active를 false로함
    m_active = false;
}

void Cuma_Main::sl_recv_signal(QJsonObject o)
{
    Cuma_Debug("sl_recv_signal(QJsonObject o) recv the signal from " + QString::number(o["From"].toInt()));

    Cuma_Debug("log env print get message", __LINE__);
    //받은 메세지를 로그 기록
    f_save_recv_json_report(o);

    QJsonObject after_bypass_protocol = mf_parse_bypass_protocol(o);

    if(after_bypass_protocol.find("no_payload") != after_bypass_protocol.end())
    {
        Cuma_Error("no_payload bypass protocol", __LINE__, m_Pid);
        return;
    }

    //recv_process layer
    f_recv_process(o);
}

void Cuma_Main::sl_start_command_signal(const QJsonObject command)
{

    Cuma_Debug("Recv Command To : " + QString::number(m_Pid), __LINE__);

    Cuma_Debug("============= command ============", __LINE__);
    Cuma_Debug(QJsonDocument(command).toJson(), __LINE__);
    try
    {
        //커맨드에 있는 파라미터를 나열하기
        if (command["command"].toString() == "file_name")
        {
            Cuma_Debug("set file name is commanded", __LINE__);

            //읽을 파일이름이 세팅이 되어있으면 현재 유닛의 읽을 파일 이름을 세팅
            QString f_name = command ["filename"].toString();

            //커맨드를 실행함 만약 리턴값이 -1일경우 Cuma_Error를 Emit함
            if ( mf_command_set_file_name(f_name) < 0)
            {
                throw Cuma_Error("mf_command_set_file_name is error", __LINE__, m_Pid);
            }
        }

        //바이패스 카운트를 세팅함
        else if (command["command"].toString() == "file_index")
        {
            Cuma_Debug("command set file index is commanded", __LINE__);

            uint32_t file_index = command["index"].toInt();

            get_File_obj()->set_File_Frag_Index(file_index);
        }

        //바이패스 카운트를 세팅함
        else if (command["command"].toString() == "bypass_count")
        {
            Cuma_Debug("command set bypass count  is commanded", __LINE__);

            m_limit_bypass_count = command["bypass_count"].toInt();

            mf_command_set_unit_bypass_count(m_limit_bypass_count);
        }

        //커맨드가 있는지 확인함

        //모든 유닛의 ping을 스크리밍 하는 테스트
        else if (command["command"].toString() == "ping")
        {
            Cuma_Debug("command ping test is commanded", __LINE__);

            //ping 커맨드를 실행함
            if (mf_command_ping_test() < 0)
            {
                throw Cuma_Error("mf_command_ping_test is error", __LINE__, m_Pid);
            }
        }

        //이 유닛에게 파일을 spread 하는 테스트
        else if (command["command"].toString() == "upload")
        {
            Cuma_Debug("command spread test is commanded", __LINE__);

            //ping 커맨드를 실행함
            if (mf_command_spread_test() < 0)
            {
                throw Cuma_Error("mf_command_spread_test is error", __LINE__, m_Pid);
            }
        }

        //이유닛에게 파일을 요청하는 메인 유닛
        else if (command["command"].toString() == "download")
        {
            Cuma_Debug("command request file test is commanded", __LINE__);

            //ping 커맨드를 실행함
            if (mf_command_req_file_test() < 0)
            {
                throw Cuma_Error("mf_command_request_file is error", __LINE__, m_Pid);
            }
        }

        //이 유닛이 파일 track 의 메인 유닛
        else if (command["command"].toString() == "bypass")
        {
            Cuma_Debug("command trace pass test is commanded", __LINE__);

            //trace pass 커맨드를 실행함
            if (mf_command_trace_pass_test() < 0)
            {
                throw Cuma_Error("mf_command_trace_pass is error", __LINE__, m_Pid);
            }
        }

        else if (command["command"].toString() == "filecheck")
        {
            Cuma_Debug("command file_check test is commanded", __LINE__);

            //trace pass 커맨드를 실행함
            if (mf_command_req_file_exsist() < 0)
            {
                throw Cuma_Error("mf_command_trace_pass is error", __LINE__, m_Pid);
            }
        }

        else
        {
            Cuma_Debug("Warning : not a command protocol", __LINE__);
        }

    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
    }
}

QJsonObject Cuma_Main::mf_parse_bypass_protocol(QJsonObject o)
{
    QJsonObject return_protocol;

    //bypass layer
    if(o.find("bypass") == o.end())
    {
        return o;
    }

    //바이패스 프로토콜을 보여줌
    Cuma_Debug("==== mf_parse_bypass_protocol ===");
    Cuma_Debug(QJsonDocument(o).toJson());
    QJsonArray bypass_chain_array = o["bypass"].toArray();

    //만약 bypass_reply일경우
    Cuma_Debug("Bypass protocol layer is dectacted", __LINE__);
    if(o["bypass_reply"].toBool() == true)
    {
        Cuma_Debug("Bypass protocol reply.. send reply_over_bypass_limit ", __LINE__);

        if(bypass_chain_array.count() < 2)
        {
            Cuma_Debug("bypass_chain count is 2 Drop the protocol", __LINE__);
            return_protocol["no_payload"] = true;
        }

        if (f_reply_over_bypass_limit(o) == 1)
        {
            Cuma_Debug("This bypass_reply is mine", __LINE__);
            return_protocol = o;
        }
        else
        {
            Cuma_Debug("Protocol Reply_Bypassing this unit : " + QString::number(m_Pid), __LINE__);
            return_protocol["no_payload"] = true;
        }
    }

    //만약 bypass_reply이 아닐경우
    else
    {
        Cuma_Debug("add bypass chain m_Pid,", __LINE__);
        QJsonArray bypass_chain_array = o["bypass"].toArray();
        bypass_chain_array.append(static_cast<int>(m_Pid));
        o["bypass"] = bypass_chain_array;

        Cuma_Debug("Bypass protocol is not reply.. send reply_over_bypass_limit ", __LINE__);
        if (f_over_bypass(o) == 1)
        {
            Cuma_Debug("bypass is limit reply the bypass_protocol");
            o["bypass_reply"] = true;

            f_reply_over_bypass_limit(o);

            return_protocol["no_payload"] = true;
        }
        else
        {
            o["bypass_reply"] = true;

            return_protocol = o;
        }
    }
    return return_protocol;
}

void Cuma_Main::client_send(QJsonObject req_send_protocol, QSharedPointer<Cuma_Main>& target_unit)
{
    if(m_bypass_protocol_layer_active == false)
    {
        emit target_unit->s_recv(req_send_protocol);
        return;
    }

    Cuma_Debug("make bypass_protocol",  __LINE__);
    QJsonObject bypass_protocol_layer = cuma_protocol::req_bypass_protocol(m_Pid, m_limit_bypass_count);
    bypass_protocol_layer["bypass_count"] = static_cast<int>(m_limit_bypass_count - 1);

    for(QJsonObject::iterator it = bypass_protocol_layer.begin(); it != bypass_protocol_layer.end(); it++)
    {
        req_send_protocol.insert(it.key(), it.value());
    }

    Cuma_Debug("==================== send protocol ====================", __LINE__);
    Cuma_Debug(QJsonDocument(req_send_protocol).toJson());

    Cuma_Debug("send bypass_protocol", __LINE__);
    emit target_unit->s_recv(req_send_protocol);
}

void Cuma_Main::server_send(QJsonObject req_send_protocol, QSharedPointer<Cuma_Main>& target_unit)
{
    emit target_unit->s_recv(req_send_protocol);
}

void Cuma_Main::server_bypass_send(QJsonObject req_send_protocol, QJsonObject recv_protocol, QSharedPointer<Cuma_Main> &target_unit)
{
    req_send_protocol["bypass"] = recv_protocol["bypass"];
    req_send_protocol["bypass_count"] = recv_protocol["bypass_count"];
    req_send_protocol["bypass_limit_count"] = recv_protocol["bypass_limit_count"];
    req_send_protocol["bypass_reply"]  = recv_protocol["bypass_reply"];

    Cuma_Debug("==================== server_send protocol ====================", __LINE__);
    Cuma_Debug(QJsonDocument(req_send_protocol).toJson());
    emit target_unit->s_recv(req_send_protocol);
}

QSharedPointer<Cuma_Main> Cuma_Main::find_bypass_reply_obj(QJsonArray arr)
{
    try
    {
        if ( arr.last().toInt() != m_Pid)
        {
            throw Cuma_Error("Unit is not in bypass array list", __LINE__, m_Pid);
        }

        int target_pid = arr.at(arr.count() - 2).toInt();

        QSharedPointer <Cuma_Main> return_unit = f_find_unit_from_inside_timeout_unit(target_pid);

        if(return_unit == nullptr)
        {
            throw Cuma_Error("Can't find return unit pid", __LINE__, m_Pid);
        }

        return return_unit;
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return nullptr;
    }
}

void Cuma_Main::f_recv_process(const QJsonObject o)
{
    try
    {

        Cuma_Debug("check json is empty", __LINE__);
        //만약 json이 없을 경우 예외 처리
        if (o.isEmpty()){ throw Cuma_Error("no recv process", __LINE__, m_Pid);}

        Cuma_Debug("print output debug messgae", __LINE__);
        //받은 메세지를 디버그 메세지 출력
        Cuma_Debug(QJsonDocument(o).toJson(), __LINE__);

        Cuma_Debug("get unit obj from pid", __LINE__);

        //파일 frag 저장일경우
        if (o["process"].toString() == "save")
        {
            Cuma_Debug("check process is save_frag", __LINE__);

            //수신 json 저장 프로세스를 먼저 실행함
            if (o["reply"].isNull() == true|| o["reply"].toBool() == false)
            {
                //파일 다운로드 프로세스
                if (f_download_file_frag_from_unit(o) < 0)
                {
                    throw Cuma_Error("f_download_file_frag_to_unit is fail", __LINE__, m_Pid);
                }
            }

            //만약 save의 reply로그가 true일경우 로그에 저장
            else
            {
                if (f_reply_download_file_frag_to_unit(o) < 0)
                {
                    throw Cuma_Error("f_reply_check_frag_to_unit_Error", __LINE__, m_Pid);
                }
            }
        }

        //파일 frag 읽기일 경우
        else if (o["process"].toString() == "read")
        {
            Cuma_Debug("check process is read_frag", __LINE__);

            //만약 응답 요청이 아닐경우 저장된 파일을 읽어서 전달을 함
            if (o["reply"].isNull() == true|| o["reply"].toBool() == false)
            {
                //유닛에게 파일 바이너리를 전송함
                if (f_upload_file_frag_from_unit(o) < 0)
                {
                    throw Cuma_Error("f_reply_check_frag_to_unit_Error", __LINE__, m_Pid);
                }
            }

            //만약 응답 요청일 경우
            else
            {
                //f_reply_upload_file_frag_to_unit의 프로시저를 실행함
                f_reply_upload_file_frag_to_unit(o);
            }
        }

        //파일 체크일경우
        else if (o["process"].toString() == "check_file")
        {

            Cuma_Debug("check process is check_file", __LINE__);

            //만약 응답 요청이 자기가 아닐경우 파일을 체크해서 리턴함
            if (o["reply"].isNull() == true|| o["reply"].toBool() == false)
            {
                f_check_file_frag_to_unit(o);
            }

            //응답 reply가 왔을 경우 해당 유닛에 파일이 있는지 판단함
            else
            {
                f_reply_check_file_frag_to_unit(o);
            }

        }

        //ping 리턴 메세지일 경우
        else if (o["process"].toString() == "ping")
        {
            Cuma_Debug("check process is ping", __LINE__);

            //응답 요청일경우 시간에 텀을 두고 reply를 함
            if (o["reply"].isNull() == true|| o["reply"].toBool() == false)
            {
                //해당 유닛와의 연결된 시간을 array에서 찾음
                uint32_t u_delay_time = m_Unit_delay_time_array[m_Pid][static_cast<uint32_t>(o["From"].toInt())];

                //응답 프로토콜을 건냄
                emit m_Cuma_unit_list[o["From"].toInt()]->s_recv(cuma_protocol::reply_ping_protocol(m_Pid, true));
            }
            //응답 reply일 경우 전송할 유닛들 리스트에 넣음
            else
            {
                //핑을 전송한 유닛의 shared_Pointer를 찾음
                QSharedPointer<Cuma_Main> temp_main =  m_Cuma_unit_list[static_cast<uint32_t>(o["From"].toInt())];

                uint32_t u_ping_time = m_Unit_delay_time_array[m_Pid][static_cast<uint32_t>(o["From"].toInt())];

                //만약 ping이 해당 시간 내에 들어왔다면 추가함
                if( ! (m_ping_limit < u_ping_time ) )
                {
                    //m_Cuma_unit_list에 추가함
                    m_Cuma_unit_inside_timeout_unit_list.append(temp_main);
                }
            }
        }

        //아닐경우 읽을수 없는 프로토콜로 디폴트 메세지를 출력
        else
        {
            Cuma_Debug("if can't read process", __LINE__);

            Cuma_Error("Can't read recv protocol \n " + QJsonDocument(o).toJson(), __LINE__, m_Pid).show_error_string();
        }
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
    }
}

int Cuma_Main::f_tell_time()
{
    //현재 시간을 알려줌
    if (unit_Timer::is_start == true)
    {
        return unit_Timer::time.elapsed();
    }

    //현재 타이머가 설정되어있지 않으므로 -1를 리턴함
    else
    {
        return -1;
    }
}


void Cuma_Main::f_push_unit(QSharedPointer<Cuma_Main> unit)
{
    QMutexLocker locker(&m_locker);

    m_Send_Unit_list.push_back(unit);
}

QSharedPointer<Cuma_Main> Cuma_Main::f_pop_unit()
{
    QMutexLocker locker(&m_locker);

    QSharedPointer<Cuma_Main> unit = m_Send_Unit_list.last();
    m_Send_Unit_list.pop_back();

    return unit;
}

void Cuma_Main::f_save_recv_json_report(QJsonObject e)
{
    //타임을 적어서 넣음
    e["To"] = static_cast<int> (m_Pid);
    e["Time"] = f_tell_time();

    //프로토콜을 recv_arr에 넣음
    QJsonArray recv_arr = m_report_json["recv"].toArray();
    recv_arr.append(e);
    m_report_json["recv"] = recv_arr;

}

/*void Cuma_Main::f_save_send_json_report(QJsonObject protocol)
{
    //타임을 적어서 넣음
    e["Time"] = f_tell_time();

    //프로토콜을 recv_arr에 넣음
    QJsonArray recv_arr = m_report_json["send"].toArray();
    recv_arr.push_back(e);
    m_report_json["send"] = recv_arr;
}
*/
