#include "cuma_main.h"

Cuma_Main::Cuma_Main(QObject *parent) : QObject(parent), redirect(0)
{

    m_File = new Cuma_File;

    //stop 시그널을 바인딩함
    connect(this, SIGNAL(s_stop_unit()), this, SLOT(sl_stop_unit()));

    //recv 시그널을 바인딩함
    connect(this, SIGNAL(s_recv(QJsonObject)), this, SLOT(sl_recv_signal(QJsonObject)));

    //s_start_spread 시그널을 바인딩함
    connect(this, SIGNAL(s_start_command(const QJsonObject)), this, SLOT(sl_start_command_signal(const QJsonObject)));

}

Cuma_Main::~Cuma_Main()
{
    //stop 시그널을 바인딩함
    disconnect(this, SIGNAL(s_stop_unit()), this, SLOT(sl_stop_unit()));

    //recv 시그널을 언바인딩함
    disconnect(this, SIGNAL(s_recv(QJsonObject)), this, SLOT(sl_recv_signal(QJsonObject)));

    //s_start_spread 시그널을 바인딩함
    disconnect(this, SIGNAL(s_start_spread(QString)), this, SLOT(sl_start_spread_signal(QString)));
}

void Cuma_Main::mf_set_unit_list(QVector<Cuma_Main> list)
{
    if (list != nullptr)
    {
        m_Cuma_unit_list = list;
    }
}

QVector<Cuma_Main> Cuma_Main::mf_get_unit_list()
{
    return m_Cuma_unit_list;
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
    m_PId = pid;
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

int Cuma_Main::mf_command_set_file_name(QString f_name)
{
    m_File->set_File_Name(f_name);
    return 0;
}

int Cuma_Main::mf_command_ping_test()
{
    try{

        //ping 테스트 하는 클라이언트

        //모든 유닛들을 ping으로 스크리밍해서 리스트 안에 넣음
        if(m_Cuma_unit_list.count() == 0)
        {
            throw Cuma_Error("mf_command_ping_test: Cuma_unit_list is empty", __LINE__, m_Pid);
        }

        //모든 유닛들에게 전송함
        foreach(QSharedPointer<Cuma_Main> p, m_Cuma_unit_unit_list)
        {
            emit p->s_recv(cuma_protocol::req_ping_protocol(m_Pid));
        }
        return 0;
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

int Cuma_Main::mf_command_spread_test()
{
    try{

        //ping limit 내의 유닛들이 있는지
        if( ! (m_Cuma_unit_inside_timeout_unit_list.count() > 0))
        {
            throw Cuma_Error("mf_command_spread_test: no unit found in Cuma_unit_inside_timeout_list ", __LINE__, m_Pid);
        }

        //전송할 파일 바이너리를 읽음
        QByteArray spread_binary =  m_File->get_File_binary();

        //만약 spread_binary가 nullptr일경우 예외처리
        if(spread_binary == nullptr)
        {
            throw Cuma_Error("mf_command_spread_test: Can't of read File bianry", __LINE__, m_Pid);
        }

        //전송할 파일들을 유닛수대로 파편화를 함
        m_File->set_File_Frag_Count(m_Cuma_unit_inside_timeout_unit_list.count());

        uint i = 0;

        //파일 정보에 대한 블록을 만듬
        QJsonObject file_info_block;
        file_info_block["file_frag_count"] = m_Cuma_unit_inside_timeout_unit_list.count();
        file_info_block["file_binary_size"] = m_File->get_File_binary().count();

        //파일의 정보 블록을 저장함
        m_file_info_block[m_File->get_File_Name()] = file_info_block;

        //파일 유닛들의 send info 저장함
        QMap<uint32_t, QVector<uint32_t>> file_save_unit;

        //파일 파편화를 모든 유닛들에게 전송함
        foreach(QSharedPointer<Cuma_Main>& p, m_Cuma_unit_unit_list)
        {
            //파편화에 대한 json을 만듬
            QJsonObject send_frag_binary;
            send_frag_binary["file_binary"] = m_File->get_File_Frag_By_Index(i);
            send_frag_binary["file_name"] = m_File->get_File_Name();
            send_frag_binary["file_index"] = i;
            send_frag_binary["file_info_block"] = file_info_block;

            emit p->s_recv(cuma_protocol::req_file_binary_save_protocol(send_frag_binary, m_Pid));

            QVector<uint32_t> file_Frag_index;
            file_Frag_index.append(p->mf_get_pid());

            file_save_unit[i] = file_Frag_index;

            i++;
        }

        //File_frag를 수신한 유닛들의 주소들을 frag인덱스별로 저장함
        m_file_frag_address[m_File->get_File_Name()] = file_save_unit;

        //유닛들에게 전송하는것을 마침
        return 0;
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

int Cuma_Main::mf_command_req_file_test()
{

    try{

        //file_frag_address의 블록이 이미 있을때 해당 유닛에게 블록을 읽는 커맨드
        //반드시 파일별 m_file_frag_address가 미리 있어야함

        //읽을 바이패스 파일의 블록을 저장할 리스트
        QVector<uint32_t> req_file_frag_index;

        QJsonObject read_list = m_file_info_block[m_File->get_File_Name()];

        uint32_t read_list_count = dynamic_cast<uint32_t> (read_list["file_index"].toInt());

        //해당 파일 블록이 있는지 체크함
        QMap<uint32_t, QVector<uint32_t>>& file_block_list = m_file_frag_address[m_File->get_File_Name()];

        //frag_index별로 유닛 벡터에 유닛들이 있는지 확인함
        for(uint32_t i = 0; i < read_list_count ; i++)
        {
            QVector<uint32_t> unit_list = file_block_list[i];

            if ( unit_list.isEmpty() == true)
            {
                //읽을 파일블록 인덱스를 read_list에 넣음
                req_file_frag_index.append(i);
            }
        }

        //만약 읽을 파일 address가 없을경우 Cuma_Error를 리턴함
        if(req_file_frag_index.isEmpty() == false)
        {
            throw Cuma_Error("mf_command_req_file_test : Need to read All File_Block", __LINE__, m_Pid);
        }

        //파일 address에 적혀있는 유닛들에게 전송함
        for(uint32_t i = 0; i < read_list_count ; i++)
        {
            QVector<uint32_t>& unit_list = file_block_list[i];

            QSharedPointer<Cuma_Main>& ready_send_unit = f_find_unit_from_inside_timeout_unit(unit_list);

            //요청 프로토콜을 unit에게 전송함
            emit ready_send_unit->s_recv(cuma_protocol::req_file_binary_read_protocol(m_File->get_File_Name(),
                                                                                      req_file_frag_index[i],
                                                                                      m_Pid));
        }

        uint32_t timeout_t = 0;

        do
        {
            //타임아웃이 될때까지 wait
            QThread::wait(1000);
            timeout_t ++;

        }while(timeout_t < 10);

        //만약 모든 frag_index의 리스트들이 돌아오지 않았을 경우 Cuma_Error를 리턴함
        if(file_block_list.count() != read_list_count)
        {
            Cuma_Error("all frag_index_list is not complete", __LINE__, m_Pid);

        }

        return 0;
    }

    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

int Cuma_Main::mf_command_trace_pass_test()
{
    //모든 유닛들에게 파일이 있는지 체크를 하는 실험

    //frag_address을 전부 flush함
    m_file_frag_address[m_File->get_File_Name()].clear();

    //모든 유닛에게 파일 메타데이터 인덱스를 요청함
    foreach(QSharedPointer<Cuma_Main>& p, m_Cuma_unit_inside_timeout_unit_list)
    {
        emit p->s_recv(cuma_protocol::req_is_file_exsist_protocol(m_File->get_File_Name(), m_Pid));
    }

    //파일 메타데이터 인덱스가 도착했는지 체크함
    if(m_file_info_block[m_File->get_File_Name()].isEmpty())
    {
        throw Cuma_Error("Can't find any File_index");
    }

    //인덱스가 도착했으면 인덱스를 읽어서 is_file_frag_exsist 프로토콜로 파일frag를 체크함

    for(uint32_t frag_index = 0;
        frag_index < m_file_info_block[m_File->get_File_Name()]["file_frag_count"].toInt();
        frag_index++)
    {
        //is_file_exsist_protocol을 모든 유닛에게 전송
        emit p->s_recv(cuma_protocol::req_is_file_exsist_protocol(frag_index, m_Pid));
    }
}

void Cuma_Main::mf_command_set_unit_bypass_count(uint32_t count)
{
    //유닛의 바이패스 카운트를 설정함
    m_limit_bypass_count = count;
}

void Cuma_Main::sl_stop_unit()
{
    //디버그 메세지
    Cuma_Debug("stop_unit is dected : Unit ID : " + m_Pid);

    //전송 유닛리스트들이 현재 남아있는지 확인함
    if (m_Send_Unit_list.count() != 0)
    {
        //디버그 메세지
        Cuma_Debug("m_Send_Unit_list is exsist flush them all : Unit ID : " + m_Pid);

        //f_pop_unit()이 nullptr를 리턴할때까지 반복함
        while(f_pop_unit() != nullptr){}

        //반복자가 완료됨을 나타내는 디버그 메세지 표시함
        Cuma_Debug("flush m_Send_Unit_list : Unit ID : " + m_Pid);
    }

    //active를 false로함
    m_active = fasle;
}

void Cuma_Main::sl_recv_signal(QJsonObject o)
{
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

            if (mf_command_set_unit_bypass_count(m_limit_bypass_count) < 0)
            {
                throw Cuma_Error("mf_command_set_unit_bypass_count is error", __LINE__, m_Pid);
            }
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
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
    }
}

void Cuma_Main::f_recv_process(const QJsonObject& o)
{
    try
    {

        //만약 json이 없을 경우 예외 처리
        if (o.isEmpty()){ throw Cuma_Error("no recv process", __LINE__, m_Pid);}

        //받은 메세지를 디버그 메세지 출력
        Cuma_Debug_protocol(o, m_Pid);

        //받은 메세지를 로그 기록
        f_save_recv_json_report(o);

        //pid로 유닛객체를 받음
        QSharedPointer<Cuma_Main> send_unit = m_Cuma_unit_list[dynamic_cast<uint32_t>(o["pid"].toInt())];

        //파일 frag 저장일경우
        if (o["proecess"].toString() == "save")
        {

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
            //만약 응답 요청이 아닐경우 저장된 파일을 읽어서 전달을 함
            if (o["reply"].isNull() == true)
            {
                //유닛에게 파일 바이너리를 전송함
                if (f_upload_file_frag_to_unit(o) < 0)
                {
                    throw Cuma_Error("f_reply_check_frag_to_unit_Error", __LINE__, m_Pid);
                }
            }

            //만약 응답 요청일 경우
            else
            {
                uint32_t rcv_unit_id = dynamic_cast<uint32_t>(o["pid"].toInt());

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
        else if (o["process"].toString() == "ping" && o["reply"].isNull() == false)
        {
            //응답 요청일경우 시간에 텀을 두고 reply를 함
            if (o["reply"].toBool() == false)
            {
                //해당 유닛와의 연결된 시간을 array에서 찾음
                uint32_t u_delay_time = m_Unit_delay_time_array[m_Pid][dynamic_cast<uint32_t>(o["From"].toInt())];

                //ping의 리미트 time대로 sleep을 함
                QThread::sleep(u_delay_time);

                //응답 프로토콜을 건냄
                emit m_Cuma_unit_list[u_delay_time]->s_recv(cuma_protocol::reply_ping_protocol(m_Pid, true));
            }
            //응답 reply일 경우 전송할 유닛들 리스트에 넣음
            else
            {
                //핑을 전송한 유닛의 shared_Pointer를 찾음
                QSharedPointer<Cuma_Main> temp_main =  m_Cuma_unit_list[dynamic_cast<uint32_t>(o["From"].toInt())];

                uint32_t u_ping_time = m_Unit_delay_time_array[m_Pid][dynamic_cast<uint32_t>(o["From"].toInt())];
                //만약 ping이 해당 시간 내에 들어왔다면 추가함
                if(m_ping_limit < u_ping_time )
                {
                    continue;
                }
                else
                {
                    //m_Cuma_unit_list에 추가함
                    m_Cuma_unit_inside_timeout_unit_list.append(temp_main);
                }
            }
        }

        //아닐경우 읽을수 없는 프로토콜로 디폴트 메세지를 출력
        else
        {
            throw Cuma_Error("Can't read recv protocol \n " + QJsonDocument(o).toJson(), __LINe__, m_Pid);
        }
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
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

    QSharedPointer<Cuma_Main> unit = m_Send_Unit_list.pop_back();

    return unit;
}

int Cuma_Main::f_send_ping_to_unit(uint32_t limit_time)
{
    try
    {
        //디버그 메세지
        Cuma_Debug("Send Ping protocol to unit to unit PID: " +  m_Pid);

        //모든유닛에게 핑 프로토콜을 전송함
        for (QVector<QSharedPointer<Cuma_Main>>::iterator it = m_Cuma_unit_list.begin(); it != m_Cuma_unit_list.end(); it++)
        {
            //emit으로 유닛들에게 시그널을 전송함
            emit (*it)->s_recv(cuma_protocol::req_ping_protocol(m_Pid));
        }

        Cuma_Debug("Wait for recv ping from Unit: PID:" +  m_Pid);
        //자기자신을 제외한 전송할 유닛리스트가 모두 모일때까지 while문으로 sleep()
        while(m_Send_Unit_list != (m_Cuma_unit_list.count() - 1))
        {
            QThread::msleep(1000);
        }

        Cuma_Debug("Scriming ping_unit: PID:" +  m_Pid);

        //전송할 유닛들 리스트 캐시
        QVector<QSharedPointer<Cuma_Main>> m_Send_Unit_list_cache;

        //리턴된 유닛들의 딜레이 핑타임 for문으로 체크해서 허용된 딜레이 핑만 스크리밍함
        for (QVector<QSharedPointer<Cuma_Main>>::iterator it = m_Send_Unit_list.begin(); it != m_Send_Unit_list.end(); it++)
        {
            //유닛들을 pop해서 해당 유닛의 핑을 체크함
            QSharedPointer<Cuma_Main> temp_unit;

            //유닛들을 먼저 pop함
            temp_unit = (*it)->f_pop_unit();

            //만약 유닛 리스트에 유닛들이 없을 경우 break으로 탈출
            if ( temp_unit == nullptr)
            {
                break;
            }

            //유닛들의 딜레이 타임을 체크함
            QVector<uint32_t> get_unit_delay_array = m_Unit_delay_time_array[m_pid];

            uint32_t delay_time = get_unit_delay_array[temp_unit->mf_get_pid()];

            //딜레이 타임이 허용되는 기준에 초과되는지 체크함
            if (delay_time > limit_time)
            {
                //만약 초과된다면 m_Cuma_unit_list에 넣지 않음
                continue;
            }

            //전송할 유닛들 캐시 리스트에 넣음
            m_Send_Unit_list_cache.append(temp_unit);
        }

        //캐시 리스트들을 핑 타임아웃 내의 유닛들의 행렬 안에 넣음
        m_Cuma_unit_inside_timeout_unit_list = m_Send_Unit_list_cache;

        //리턴함
        return 0;

    }catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

void Cuma_Main::f_save_recv_json_report(QJsonValue e)
{
    //타임을 적어서 넣음
    e["Time"] = unit_Timer::time.elapsed();

    //프로토콜을 recv_arr에 넣음
    QJsonArray& recv_arr = m_report_json["recv"].toArray();
    recv_arr.append(e);
}

void Cuma_Main::f_save_send_json_report(QJsonValue e)
{
    //타임을 적어서 넣음
    e["Time"] = unit_Timer::time.elapsed();

    //프로토콜을 recv_arr에 넣음
    QJsonArray& send_arr = m_report_json["send"].toArray();
    send_arr.append(e);
}

int Cuma_Main::f_upload_file_frag_from_unit(QJsonObject o)
{
    try
    {
        //해당 파일을 요청한 uid의 유닛 포인터를 반환함
        QSharedPointer<Cuma_Main> req_unit = m_Cuma_unit_inside_timeout_unit_list[dynamic_cast<uint32_t>(o["pid"].toInt())];

        //만약 파일을 요청한 uid의 유닛포인터가 m_Cuma_unit_list의 검색에 없을경우 예외처리
        if (req_unit == nullptr)
        {
            throw Cuma_Error("No Such Unit in m_Cuma_unit_list while replying the unit Pointer", __LINE__, m_Pid);
        }

        //요청된 저장된 파일을 읽음
        QByteArray frag_binary = m_File->get_fragment_slite(o["file_index"].toInt());

        //해당 유닛에 전송함
        req_unit->s_recv(cuma_protocol::reply_file_binary_read_protocol(o["file_name"].toString(), o["file_index"].toInt(), m_Pid, frag_binary));

    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

int Cuma_Main::f_reply_upload_file_frag_to_unit(const QJsonObject o)
{
    try
    {
        //유닛의 목적지가 어디인지
        uint32_t unit_to  =  dynamic_cast<uint32_t>(o["To"].toInt());

        uint32_t next_unit_pid = nullptr;

        //만약 자기 유닛에게 오지 않았을 경우 적혀진 경로대로 탐색을 해서 자기 유닛이후의 다음 유닛에게 전송을 함
        if (unit_to != m_Pid)
        {
            //적혀진 경로 를 json_array로 변환함
            QJsonArray bypass_array= o["bypass"].toArray();

            //bypassarray에 있는 유닛들을 찾음
            for (int i = 0; i< bypass_array.count(); i++)
            {
                //만약 해당 array가 자기 pid랑 일치할경우 next_unit_pid에 값을 할당함
                if (bypass_array.at(i).toInt() == m_Pid)
                {
                    next_unit_pid = dynamic_cast<uint32_t>(bypass_array[i+1].toInt());
                }
            }

            //next_unit_pid가 없을경우 예외처리
            if (next_unit_pid == nullptr)
            {
                throw Cuma_Error("f_reply_upload_file_frag_to_unit::no such unit from destination", __LINE__, m_Pid);
            }

            //next_unit_pid의 유닛들의 포인터를 통해 전송함
            QSharedPointer<Cuma_Main> next_bypass_unit = nullptr;

            //유닛의 pid를 찾아서 next_bypass_unit에 넣음
            for (QVector<QSharedPointer<Cuma_Main>>::iterator it = m_Cuma_unit_inside_timeout_unit_list.first(); it != m_Cuma_unit_inside_timeout_unit_list.end(); it++)
            {
                if ((*it)->mf_get_pid == next_unit_pid)
                {
                    next_bypass_unit = (*it);
                    break;
                }
            }

            //받은 recv프로토콜을 그대로 다음 유닛에게 전송
            next_bypass_unit->s_recv(o);
        }

        //만약 자기 유닛일 경우 전송을 받아서 해석을 한후 저장함
        else
        {
            //어디유닛에서 전송을 했는지 적음
            uint32_t    unit_from = dynamic_cast<uint32_t>(o["From"].toInt());

            //파일이름을 표시함
            QString     file_name = o["file_name"].toString();
            uint32_t    file_index = (uint32_t)o["file_index"].toInt();
            uint32_t    file_size = (uint32_t)o["file_byte"].toInt();
            QString     frag_string = o["file_frag"].toString();
            QByteArray  file_frag(frag_string.constData(), frag_index);

            //디버그 표시
            Cuma_Debug("f_reply_upload_file_frag_to_unit", __LINE__);
            Cuma_Debug("File_Frag_size : " + QString::number(file_size), __LINE__);
            Cuma_Debug("File_Frag_info : ", + "size : " + QString::number(file_frag.count()), __LINE__);

            //만약 frag인덱스와 file_frag의 사이즈가 안맞을 경우 예외 처리 보냄
            //이미 파일체크 프로세스가 바이패스 프로세스를 호출해서 이미 ping이 남아있는데 없다는것은 예외처리를 통해서 exception문을 호출해야함
            if (frag_index != file_frag.count())
            {
                throw Cuma_Error("f_reply_upload_file_frag_to_unit error frag_index != file_frag.count() : recv frag_index and file_frag_count is not match", __LINE__, m_Pid);
            }

            //리턴된 파일의 바이너리를 읽고 저장함
            int ret_type = m_File->save_File_Frag(file_frag, file_name, file_index);

            //만약 파일이 이미 존재할경우 continue함
            if ( ret_type == Cuma_File::Cuma_File_Status::C_F_Exsist)
            {
                continue;
            }

            //리턴함
            return 0;
        }
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

int Cuma_Main::f_download_file_frag_from_unit(QJsonObject o)
{
    try
    {
        //가지고 온 파일을 저장함
        if (m_File->save_File_Frag(o["file_binary"].toString(), o["file_name"].toString(), dynamic_cast<uint32_t>(o["file_index"].toInt())) < 0)
        {
            Cuma_Debug("Can't open file", __LINE__);
            return 0;
        }

        //전송을 한 유닛의 sharedpointer를 가짐
        QSharedPointer<Cuma_Main> prepare_send_unit = nullptr;

        //유닛의 pid를 찾아서 prepare_send_unit 넣음
        for (QVector<QSharedPointer<Cuma_Main>>::iterator it = m_Cuma_unit_inside_timeout_unit_list.first(); it != m_Cuma_unit_inside_timeout_unit_list.end(); it++)
        {
            if ((*it)->mf_get_pid == next_unit_pid)
            {
                next_bypass_unit = (*it);
                break;
            }
        }

        //만약 prepare_send_unit이 nullptr일경우 예외처리 exception문을 호출
        if (prepare_send_unit == nullptr)
        {
            throw Cuma_Error("f_reply_upload_file_frag_to_unit error frag_index != file_frag.count() : recv frag_index and file_frag_count is not match", __LINE__, m_Pid);
        }

        //전송을 한 유닛에게 reply_download_file_frag 프로토콜을 전송함
        emit prepare_send_unit->s_recv(cuma_protocol::reply_file_binary_save_protocol(o["file_name"].toString(), m_Pid, o["file_index"].toInt()));

    }

    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

int Cuma_Main::f_reply_download_file_frag_to_unit(QJsonObject o)
{
    //reply된 download_frag_file를 체크하고 파일이 저장을 한 유닛의 pid를 File_Frag의 메타데이터에 저장함
    if ( !o["reply"].isNull() && o["reply"].toBool() == true)
    {
        Cuma_Debug("Unit : " + dynamic_cast<uint32_t>(o["From"].toInt()) + "save file_frag  " + o["file_name"].toString() + ": " + o["file_index"].toInt());
    }

    //만약 아닐경우 error를 출력함(이건 reply중요도가 떨어지기때문에 굳이 exception을 던지지 않음)
    else
    {
        Cuma_Debug("f_reply_download_file_Frag is error", __LINE__, m_Pid);
    }
    return 0;
}

int Cuma_Main::f_check_file_frag_to_unit(QJsonObject &o)
{
    //만약 파일이 없을경우 다른 유닛들에게 파일을 전송하고 reply로 파일이 없고 현재 bypass count대로 탐색중이라는것을 알림

    QString req_frag_name = o["file_name"].toString();
    uint32_t req_frag_index = dynamic_cast<uint32_t>(o["file_index"].toInt());

    int32_t f_flag = m_File->read_file_frag(req_frag_name, req_frag_index);

    //파일이 없을경우 다른 유닛에게 bypass 프로토콜을 실행함
    if ( f_flag == Cuma_File::Cuma_File_Status::C_F_Dir_Not_Open || f_flag == Cuma_File::Cuma_File_Status::C_F_not_open)
    {
        //바이패스 프로토콜을 실행함
        if (f_over_bypass(o) < 0)
        {
            //만약 바이패스 프로토콜이 -1일경우 바이패스 리미트가 초과이므로 파일이 없다는 프로토콜을 만들어서 위에 있는 유닛으로 bypass 함
            QJsonObject reply_bypass_protocol = cuma_protocol::reply_is_file_exsist_protocol(o["file_index"].toInt(), m_PId, false);
            reply_bypass_protocol["bypass"] = o["bypass"].toArray();
            reply_bypass_protocol["bypass_limit_count"] = o["bypass_limit_count"].toInt();
            reply_bypass_protocol["bypass_count"] = o["bypass_count"].toInt();

            //bypass 체인으로 올라가는데 만약 에러가 발생시 예외 처리 함
            if (f_reply_over_bypass_limit(reply_bypass_protocol) < 0)
            {
                throw Cuma_Error("bypass_chain_error", __LINE__, m_Pid);
            }
        }
    }

    //파일이 있을경우 파일이 있다는 프로토콜을 보냄
    else
    {
        //전송을 한 유닛의 sharedpointer를 가짐
        QSharedPointer<Cuma_Main> prepare_send_unit = nullptr;

        //유닛의 pid를 찾아서 prepare_send_unit 넣음
        for (QVector<QSharedPointer<Cuma_Main>>::iterator it = m_Cuma_unit_inside_timeout_unit_list.first(); it != m_Cuma_unit_inside_timeout_unit_list.end(); it++)
        {
            if ((*it)->mf_get_pid == next_unit_pid)
            {
                prepare_send_unit = (*it);
                break;
            }
        }

        //프로토콜을 전송함
        emit prepare_send_unit->s_recv(cuma_protocol::reply_is_file_exsist_protocol(req_frag_name, m_Pid, true));

    }
    return 0;
}

int Cuma_Main::f_reply_check_file_frag_to_unit(QJsonObject o)
{
    try{
        //bypass의 프로토콜은 bypass_count 끝날때까지 리턴값을 못받는것으로 산정함

        //만약 bypass 프로토콜인지 확인함
        if (!o["bypass"].isNull())
        {
            uint32_t temp_bypass_count = o["bypass_count"].toInt();

            //만약 자기 로 오는 bypass 프로토콜일 경우 파일이 존재하는지 확인
            if(o["To"].toInt() == m_Pid)
            {
                //만약 파일 frag가 존재할경우 path 블록을 만들어서 저장함
                if (o["exsist"].toBool() == true)
                {
                    //bypass의 유닛들을 저장할 주소
                    QVector<uint32_t> path_unit_block;

                    QJsonArray bypass_unit_array = o["bypass"].toArray();

                    //bypass_unit_array를 path_unit_block으로 넣음
                    foreach(QJsonValue& e, bypass_unit_array)
                    {
                        path_unit_block.append(dynamic_cast<uint32_t>(e.toInt()));
                    }

                    //path_unit_block을 frag_address 저장함
                    QMap<uint32_t, QVector<uint32_t>> frag_index_address;
                    frag_index_address[dynamic_cast<uint32_t>(o["file_index"].toInt())] = path_unit_block;

                    //m_file_frag_address 저장함
                    m_file_frag_address[o["file_name"].toString()] = frag_index_address;
                }

                //만약 파일이 못찾을 경우 frag_address 저장
                else
                {
                    //못찾은 경우로 해당 frag_index에 0을 저장함
                    QMap<uint32_t, QVector<uint32_t>> frag_index_address;
                    frag_index_address[dynamic_cast<uint32_t>(o["file_index"].toInt())] = 0;

                    //m_file_frag_address 저장함
                    m_file_frag_address[o["file_name"].toString()] = frag_index_address;
                }
            }

            //자기로 오는 bypass 프로토콜이 아닐 경우 f_repoly_over_bypass_limit()를 호출
            else
            {
                if (f_reply_over_bypass_limit(o) < 0)
                {
                    throw Cuma_Error("f_reply_over_bypass_limit error", __LINE__, m_Pid);
                }
            }
        }

        //만약 바이패스가 아니면
        else
        {
            //만약 파일이 있으면 해당파일을 m_file_frag_address에 저장함
            if (o["exsist"].toBool() == true)
            {
                QMap<uint32_t, QVector<uint32_t>> frag_index_address;

                frag_index_address[dynamic_cast<uint32_t>(o["file_index"].toInt())] = dynamic_cast<uint32_t>(o["From"].toInt());

                //m_file_frag_address 저장함
                m_file_frag_address[o["file_name"].toString()] = frag_index_address;
            }

            //만약 파일이 없으면 m_file_frag_address 에 0을 저장함
            else
            {
                //못찾은 경우로 해당 frag_index에 0을 저장함
                QMap<uint32_t, QVector<uint32_t>> frag_index_address;
                frag_index_address[dynamic_cast<uint32_t>(o["file_index"].toInt())] = 0;

                //m_file_frag_address 저장함
                m_file_frag_address[o["file_name"].toString()] = frag_index_address;
            }
        }
        return 0;
    }

    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

int Cuma_Main::f_over_bypass(QJsonObject o)
{
    //자기가 가지고 있는 다른 유닛들에게 프로토콜을 전송함

    //만약 기존의 바이패스가 없는 경우 새로운 bypass를 삽입함
    if(o["bypass"].isNull())
    {
        //새로운 바이패스 프로토콜을 생성함
        o["bypass_count"] = o["bypass_limit_count"].toInt();

        //바이패스 에 자기 PId를 넣음
        o["bypass"] = o["bypass"].toArray().append(m_Pid);
    }

    //만약 bypass의 카운트가 0일경우 이미 리미트까지 갔다는 의미
    if(o["bypass_count"].toInt() == 0)
    {
        //bypass 카운트가 0이므로 -1를 리턴함
        return -1;
    }

    //bypass_count를 -1함
    o["bypass_count"] -= 1;

    //ping으로 스크리밍이 된 모든 유닛들에게 bypass함
    for (QVector<QSharedPointer<Cuma_Main>>::iterator it = m_Cuma_unit_inside_timeout_unit_list.first();
         it != m_Cuma_unit_inside_timeout_unit_list.end();
         it++)
    {
        emit (*it)->s_recv(o);
    }

    return 0;
}

int Cuma_Main::f_reply_over_bypass_limit(QJsonObject o)
{

    //리플라이가 된 프로토콜을 다른 유닛에게 넘기는 프로토콜

    try{

        //bypass count가 초과가 됬는지 확인
        if(o["bypass_count"].toInt() < 0)
        {
            //만약 초과가 됬다면 에러가 발생한것으로 extion을 발생함
            throw Cuma_Error("f_reply_over_bypass_limit: bypass_ount < bypass_limit" , __LINE__, m_Pid);
        }

        //초과가 되지 않았다면 bypass 안에 자기 pid의 체인 위에 있는 유닛에게 전송함
        QJsonArray bypass_chain =  o["bypass"].toArray();

        //전송하려는 유닛의 pid를 찾음
        uint32_t send_unit_id;

        //전송이 정상적으로 완료가 되었는지
        bool bypass_success = false;

        //전송하려는 유닛 주소
        QSharedPointer<Cuma_Main> send_unit = nullptr;

        //bypass_chain을 통해서 전송할 다음 바이패스 체인을 찾음
        for (uint32_t i = 0; i < bypass_chain.count() ; i++)
        {
            //만약 바이패스 체인에 자신을 찾았을 경우 다음 바이패스 체인인의 pid를 찾음
            if(bypass_chain.at(i) == m_Pid)
            {
                if(i == 0)
                {
                    //만약 i 가 0일때 자기자신이므로  0을 리턴함
                    return 0;
                }

                //뒤의 바이패스 체인을 찾음
                send_unit_id = bypass_chain.at(i - 1).toInt();
                break;
            }
        }

        //뒤의 바이패스 체인을 자기 유닛 리스트에서 찾아서 전송함
        for(QVector<QSharedPointer<Cuma_Main>>::iterator it = m_Cuma_unit_inside_timeout_unit_list.begin(); it != m_Cuma_unit_inside_timeout_unit_list.end(); it++)
        {
            //만약 유닛 리스트중에 보내려는 유닛을 찾았을떄
            if ((*it)->mf_get_pid() == send_unit_id)
            {
                send_unit = (*it);
                bypass_success = true;
                break;
            }
        }

        //만약 바이패스 유닛들을 못찾았을때 exception을 던짐
        if(bypass_success == false || unit_addr = nullptr)
        {
            throw Cuma_Error("f_reply_over_bypass_limit : Can't find bypass unit in this unit list", __LINE__, m_Pid);
        }

        //프로토콜을 전송함
        emit send_unit->s_recv(o);

        return 0;
    }

    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

QJsonObject cuma_protocol::req_unit_command_protocol(QString command)
{
    QJsonObject command_protocol;
    //만약 커맨드가 set_file_name일때
    if(command == "set_file_name" || command == "set_bypass_count")
    {
        Cuma_Debug("this command is not used in this command");
        Cuma_Debug("try to use cuma_protocol::req_unit_command_protocol(QString command, uint32_t count)");
        return nullptr;
    }

    if(command == "ping")
    {
        command_protocol["command_ping_test"] = true;
    }

    if(command == "spread")
    {
        command_protocol["command_spread_test"] = true;
    }

    if(command == "request")
    {
        command_protocol["command_rq_file"] = true;
    }

    if(command == "track")
    {
        command_protocol["command_trace_pass"] = true;
    }

    else
    {
        command_protocol = nullptr;
    }

    return command_protocol;
}

QJsonObject cuma_protocol::req_unit_command_protocol(QString command, uint32_t count)
{
    QJsonObject command_protocol;

    if(command == "set_file_name")
    {
        command_protocol["command_set_unit_bypass_count"] = count;
    }
    else
    {
        command_protocol = nullptr;
    }
    return command_protocol;
}

QJsonObject cuma_protocol::req_unit_command_protocol(QString command, QString name)
{
    QJsonObject command_protocol;

    if(command == "set_file_name")
    {
        command_protocol["command_set_file_name"] = name;
    }
    else
    {
        command_protocol = nullptr;
    }
    return command_protocol;
}

QJsonObject cuma_protocol::req_ping_protocol(uint32_t unit_id, bool reply)
{
    QJsonObject recv_obj = basic_command_protocol(unit_id);

    recv_obj["process"] = "ping";
    recv_obj["reply"] = is_return;
}

QJsonObject cuma_protocol::req_is_file_exsist_protocol(uint32_t file_frag_index, uint32_t unit_id)
{

}

QJsonObject cuma_protocol::req_is_file_exsist_protocol(QString f_name, uint32_t unit_id, bool req_file_index)
{

}

QJsonObject cuma_protocol::req_file_binary_save_protocol(QJsonObject file_binary, uint32_t unit_id)
{

}

QJsonObject cuma_protocol::req_file_binary_read_protocol(QString binary_name, uint32_t file_frag_index, uint32_t unit_id)
{

}

QJsonObject cuma_protocol::reply_ping_protocol(uint32_t unit_id, bool is_return)
{

}

QJsonObject cuma_protocol::reply_is_file_exsist_protocol(uint32_t file_frag_index, uint32_t unit_id, bool is_exsist)
{

}

QJsonObject cuma_protocol::reply_file_binary_save_protocol(QString file_frag_name, uint32_t unit_id, uint32_t frag_index)
{

}

QJsonObject cuma_protocol::reply_file_binary_read_protocol(QString file_frag_name, uint32_t file_frag_index, uint32_t unit_id, QByteArray binary)
{

}
