#include "cuma_main.h"

uint32_t Cuma_Main::m_limit_bypass_count = 0;

bool unit_Timer::is_start = false;

QTime unit_Timer::time;

Cuma_Main::Cuma_Main(QObject *parent) : QObject(parent)
{

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

#ifndef TEST
    //recv 시그널을 바인딩함
    connect(this, SIGNAL(s_recv(QJsonObject)), this, SLOT(sl_recv_signal(QJsonObject)));
#endif

#ifdef TEST
    connect (this, SIGNAL(s_recv(QJsonObject)), this, SLOT(sl_recv_test(const QJsonObject)));
#endif

    //s_start_spread 시그널을 바인딩함
    connect(this, SIGNAL(s_start_command(const QJsonObject)), this, SLOT(sl_recv_test(const QJsonObject)));

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

#ifdef TEST
    disconnect (this, SIGNAL(s_recv_test(QJsonObject)), this, SLOT(sl_recv_test(const QJsonObject)));
#endif
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

QSharedPointer<Cuma_File> Cuma_Main::get_File_obj()
{
    return m_File;
}

void Cuma_Main::mf_t_set_limit_unit(QVector<QSharedPointer<Cuma_Main> > &v)
{
    m_Cuma_unit_inside_timeout_unit_list = v;
}

void Cuma_Main::set_bypass_limit_count(uint32_t limit)
{
    m_limit_bypass_count = limit;
}

uint32_t Cuma_Main::get_bypass_limit_count()
{
    return m_limit_bypass_count;
}

void Cuma_Main::sl_stop_unit()
{
    //디버그 메세지
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
    Cuma_Debug("sl_recv_signal(QJsonObject o) recv the signal from " + o["From"].toString());

    //만약 recv_signal을 받았을 경우 f_recv_process에서 recv을 분석해서 호출함
    f_recv_process(o);
}

void Cuma_Main::sl_start_command_signal(const QJsonObject command)
{

    try
    {
        //커맨드에 있는 파라미터를 나열하기
        if (command["command_set_file_name"].isNull() == false)
        {
            //읽을 파일이름이 세팅이 되어있으면 현재 유닛의 읽을 파일 이름을 세팅
            QString f_name = command ["command_set_file_name"].toString();

            //커맨드를 실행함 만약 리턴값이 -1일경우 Cuma_Error를 Emit함
            if ( mf_command_set_file_name(f_name) < 0)
            {
                throw Cuma_Error("mf_command_set_file_name is error", __LINE__, m_Pid);
            }
        }

        //바이패스 카운트를 세팅함
        if (command["command_set_unit_bypass_count"].isNull() == false)
        {
            m_limit_bypass_count = command["command_set_unit_bypass_count"].toInt();

            mf_command_set_unit_bypass_count(m_limit_bypass_count);
        }

        //커맨드가 있는지 확인함

        //모든 유닛의 ping을 스크리밍 하는 테스트
        if (command["command_ping_test"].isNull() == false)
        {
            //ping 커맨드를 실행함
            if (mf_command_ping_test() < 0)
            {
                throw Cuma_Error("mf_command_ping_test is error", __LINE__, m_Pid);
            }
        }

        //이 유닛에게 파일을 spread 하는 테스트
        if (command["command_spread_test"].isNull() == false)
        {
            //ping 커맨드를 실행함
            if (mf_command_spread_test() < 0)
            {
                throw Cuma_Error("mf_command_spread_test is error", __LINE__, m_Pid);
            }
        }

        //이유닛에게 파일을 요청하는 메인 유닛
        if (command["command_rq_file"].isNull() == false)
        {
            //ping 커맨드를 실행함
            if (mf_command_req_file_test() < 0)
            {
                throw Cuma_Error("mf_command_spread_test is error", __LINE__, m_Pid);
            }
        }

        //이 유닛이 파일 track 의 메인 유닛
        if (command["command_trace_pass"].isNull() == false)
        {
            //ping 커맨드를 실행함
            if (mf_command_trace_pass_test() < 0)
            {
                throw Cuma_Error("mf_command_spread_test is error", __LINE__, m_Pid);
            }
        }

        //만약 없다면 에러 메세지를 출력함
        if(command["command_set_file_name"].isNull() != false ||
                command["command_set_unit_bypass_count"].isNull() != false||
                command["command_ping_test"].isNull() != false||
                command["command_spread_test"].isNull() != false||
                command["command_rq_file"].isNull() != false||
                command["command_trace_pass"].isNull() != false)
        {
            //에러 메세지 출력
            Cuma_Debug("Warning : not a protocol", __LINE__);
        }
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
    }
}

void Cuma_Main::sl_recv_test(QJsonObject o)
{

}

void Cuma_Main::f_recv_process(const QJsonObject& o)
{
    try
    {

        Cuma_Debug("check json is empty", __LINE__);
        //만약 json이 없을 경우 예외 처리
        if (o.isEmpty()){ throw Cuma_Error("no recv process", __LINE__, m_Pid);}

        Cuma_Debug("print output debug messgae", __LINE__);
        //받은 메세지를 디버그 메세지 출력
        Cuma_Debug_protocol(o, m_Pid);

        Cuma_Debug("log env print get message", __LINE__);
        //받은 메세지를 로그 기록
        f_save_recv_json_report(o);

        Cuma_Debug("get unit obj from pid", __LINE__);
        //pid로 유닛객체를 받음
        //QSharedPointer<Cuma_Main> send_unit = m_Cuma_unit_list[static_cast<uint32_t>(o["pid"].toInt())];

        //파일 frag 저장일경우
        if (o["proecess"].toString() == "save")
        {
            Cuma_Debug("check process is save_frag", __LINE__);

            //수신 json 저장 프로세스를 먼저 실행함
            if (o["reply"].isNull() == true)
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
            if (o["reply"].isNull() == true)
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
                uint32_t rcv_unit_id = static_cast<uint32_t>(o["pid"].toInt());

                //받으려는 유닛이 자기가 아닐경우
                if ( rcv_unit_id != m_Pid)
                {
                    //프로토콜에 명시된 다음 바이패스로 전달함
                    f_over_bypass(o);
                }

                //받으려는 유닛이 자기일경우
                else
                {
                    //f_reply_upload_file_frag_to_unit의 프로시저를 실행함
                    f_reply_upload_file_frag_to_unit(o);
                }
            }
        }

        //파일 체크일경우
        else if (o["process"].toString() == "check_file")
        {

            Cuma_Debug("check process is check_file", __LINE__);

            //만약 응답 요청이 자기가 아닐경우 파일을 체크해서 리턴함
            if (o["reply"].isNull() == true)
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
            if (o["reply"].toBool() == false)
            {
                //해당 유닛와의 연결된 시간을 array에서 찾음
                uint32_t u_delay_time = m_Unit_delay_time_array[m_Pid][static_cast<uint32_t>(o["From"].toInt())];

                //ping의 리미트 time대로 sleep을 함
                QThread::sleep(u_delay_time);

                //응답 프로토콜을 건냄
                emit m_Cuma_unit_list[u_delay_time]->s_recv(cuma_protocol::reply_ping_protocol(m_Pid, true));
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

            throw Cuma_Error("Can't read recv protocol \n " + QJsonDocument(o).toJson(), __LINE__, m_Pid);
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
    e["Time"] = f_tell_time();

    //프로토콜을 recv_arr에 넣음
    QJsonArray recv_arr = m_report_json["recv"].toArray();
    recv_arr.push_back(e);
    m_report_json["recv"] = recv_arr;

}
