#include "cuma_main.h"

Cuma_Main::Cuma_Main(QObject *parent) : QObject(parent)
{

    //stop 시그널을 바인딩함
    connect(this, SIGNAL(s_stop_unit()), this, SLOT(sl_stop_unit()));

    //recv 시그널을 바인딩함
    connect(this, SIGNAL(s_recv(QJsonObject)), this, SLOT(sl_recv_signal(QJsonObject)));

    //s_start_spread 시그널을 바인딩함
    connect(this, SIGNAL(s_start_spread(QString)), this, SLOT(sl_start_spread_signal(QString)));


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
    if(list != nullptr)
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

void Cuma_Main::sl_stop_unit()
{
    //디버그 메세지
    Cuma_Debug("stop_unit is dected : Unit ID : " + m_Pid);

    //전송 유닛리스트들이 현재 남아있는지 확인함
    if(m_Send_Unit_list.count() != 0)
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

void Cuma_Main::sl_start_spread_signal(QString filename)
{

    try
    {

        //유닛에게 핑을 전송하여 해당 유닛들이 반응을 하는지 체크함
        foreach (QSharedPointer<Cuma_Main> unit, m_Cuma_unit_list) {

            //모든 유닛들에게 ping 프로토콜을 전송함
            emit unit->s_recv(cuma_protocol::req_ping_protocol(m_Pid));
        }

        Cuma_Debug("check unit_list delay_time");
        //저장되어있는 유닛리스트의 delay time을 체크함
        f_send_ping_to_unit(m_ping_limit);

        Cuma_Debug("read file_binary");
        //파일의 바이너리를 읽음
        //파일바이너리 읽기가 실패할 경우 파일 에러 메세지를 읽어서 throw함
        if (f_f_read_file(file_name) < 0){ throw Cuma_Error("Error of file_binary_read :" + m_File->mf_get_file_error_string(), __LINE__, m_Pid);}

        Cuma_Debug("frag the file");
        //파일 바이너리를 파편화 시킴
        if(f_fragment_file(file_) < 0){ throw Cuma_Error("Error of file_binary_fragment :" + m_File->mf_get_file_error_string(), __LINE__, m_Pid);}

        //바이너리 파편화를 가지고 옴
        QVector<QByteArray> t_file_frag_binary = m_File->get_all_fragment_file();

        Cuma_Debug("check valid file_frag");
        //file이 valid 한지 확인
        if(t_file_frag_binary.count() == 0){ throw Cuma_Error("File_frag_binary has no frag");}

        //파편화를 모든 유닛들에게 전송함
        for(uint32_t i = 0; i < m_Cuma_unit_list.count(); i++)
        {

            QJsonObject obj;
            obj["file_binary"] = t_file_frag_binary[i];
            obj["file_frag_index"] = (i % m_File->get_frag_count());
            obj["file_name"] = filename;

            //디버그 출력
            Cuma_Debug(QJsonDocument(obj).toJson().toStdString());

            //obj을 유닛에게 전송함
            m_Cuma_unit_list[i]->s_recv(cuma_protocol::req_file_binary_save_protocol(obj, m_Pid));
        }

        //전송 완료 디버그 메세지 출력
        Cuma_Debug("spread signal compolete");

        return 0;
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

void Cuma_Main::f_recv_process(const QJsonObject& o)
{
    try
    {
        //수신 프로세스

        //만약 json이 없을 경우 예외 처리
        if(o.isEmpty()){ throw Cuma_Error("no recv process", __LINE__, m_Pid);}

        //파일 프로세스 설정

        //파일 프래그 저장일경우
        if(o["proecess"].toString() == "save")
        {

            if(o["reply"].toBool() == false)
            {
                QByteArray file_frag_binary = o["file_binary"].toString();

                uint32_t file_frag_size = file_frag_binary.size();

                //파일을 읽기
                m_File->read_file_frag(file_frag_binary, file_frag_size);

                //받은 pid의 핑타임을 저장함
                QVector<uint32_t> ping_time_arr = m_Unit_delay_time_array[m_Pid];
                uint32_t unit_ping_time = ping_time_arr[o["pid"].toInt()];


                //저장완료라는 json을 클라이언트에게 전달함
                QJsonObject reply_recv;
                reply_recv["from"] = m_Pid;
                reply_recv["to"] = o["from"].toInt();
                reply_recv["process"] = "save";
                reply_recv["reply"] = true;
                reply_recv["file_index"] = o["file_index"].toInt();
                reply_recv["file_name"] = o["file_name"].toString();
                reply_recv["file_byte"] = file_frag_size;
                reply_recv["time"] = f_tell_time();

                //해당 pid의 유닛을 찾고 전송함
                QSharedPointer<Cuma_Main> recv_unit =  m_Cuma_unit_list[dynamic_cast<uint32_t>(o["from"].toInt())];

                //만약 파일을 요청한 uid의 유닛포인터가 m_Cuma_unit_list의 검색에 없을경우 예외처리
                if(recv_unit == nullptr)
                {
                    throw Cuma_Error("No Such Unit in m_Cuma_unit_list while replying the unit Pointer", __LINE__, m_Pid);
                }

                //전송
                emit recv_unit->s_recv(reply_recv);

                //json분석 로그를 저장함
                QJsonValue save_log;

                //로그 json을 구성함
                save_log["from"] = dynamic_cast<uint32_t> (o["from"].toInt());
                save_log["to"] = m_Pid;
                save_log["process"] = "save";
                save_log["ping_time"] = unit_ping_time;
                save_log["First_recv_time"] = f_tell_time();
                save_log["file_byte"] = file_frag_size;
                save_log["file_name"] = o["file_name"].toString();
                save_log["file_index"] = o["file_index"].toInt();
                save_done["time"] = f_tell_time();


                //로그 json을 push함
                m_report_json["recv"].toArray().append(save_log);

            }

            //만약 save의 reply로그가 true일경우 로그에 저장
            else
            {
                //리플라이된 json을 리포트에 push함
                m_report_json["recv"].toArray().append(o);
            }

        }

        //파일 프래그 읽기일경우
        else if(o["process"].toString() == "read")
        {
            //만약 응답 요청이 아닐경우 저장된 파일을 읽어서 전달을 함
            if(o["reply"] == false)
            {

                //해당 파일을 요청한 uid의 유닛 포인터를 반환함
                QSharedPointer<Cuma_Main> req_unit = m_Cuma_unit_list[dynamic_cast<uint32_t>(o["pid"].toInt())];

                //만약 파일을 요청한 uid의 유닛포인터가 m_Cuma_unit_list의 검색에 없을경우 예외처리
                if(req_unit == nullptr)
                {
                    throw Cuma_Error("No Such Unit in m_Cuma_unit_list while replying the unit Pointer", __LINE__, m_Pid);
                }

                //요청된 저장된 파일을 읽음
                QByteArray frag_binary = m_File->get_fragment_slite(req_file_index);

                //만약 파일이 없다면 연결된 다른 유닛들에게 요청된 파일을 읽을 명령을 전송함


                uint32_t req_file_index = o["file_index"].toInt();
                uint32_t file_frag_size = frag_binary.size();

                //해당 uid에 읽은 파일 fragment를 json형태로 만듬
                QJsonObject reply_recv;
                reply_recv["from"] = m_Pid;
                reply_recv["to"] = o["from"].toInt();
                reply_recv["process"] = "read";
                reply_recv["reply"] = true;
                reply_recv["file_binary"] = frag_binary;
                reply_recv["file_index"] = req_file_index;
                reply_recv["file_name"] = o["file_name"].toString();
                reply_recv["file_byte"] = file_frag_size;
                reply_recv["time"] = f_tell_time();

                //해당 uid의 파일을 전송함
                emit req_unit->s_recv(reply_recv);


            }

            //만약 응답요청일 경우 리포트에 저장함
            else
            {

            }
        }

        //파일 체크일경우
        else if(o["process"].toString() == "check_file")
        {

        }

        //ping 리턴 메세지일 경우
        else if(o["process"].toString() == "ping")
        {

        }

        //아닐경우 디폴트 메세지를 출력함
        else
        {

        }
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}

void Cuma_Main::f_write_report_json(QJsonObject o)
{

}

int Cuma_Main::f_tell_time()
{

}

int Cuma_Main::f_start_spread(const QString file_name)
{

}

int Cuma_Main::f_f_read_file(const QString file_name)
{

}

int Cuma_Main::f_fragment_file(const uint32_t count)
{

}

void Cuma_Main::f_push_unit(QSharedPointer<Cuma_Main> unit)
{

}

QSharedPointer<Cuma_Main> Cuma_Main::f_pop_unit()
{
    //뮤텍스로
}

int Cuma_Main::f_send_ping_to_unit(uint32_t limit_time)
{
    try
    {
        //디버그 메세지
        Cuma_Debug("Send Ping protocol to unit to unit PID: " +  m_Pid);

        //모든유닛에게 핑 프로토콜을 전송함
        for(QVector<QSharedPointer<Cuma_Main>>::iterator it = m_Cuma_unit_list.begin(); it != m_Cuma_unit_list.end(); it++)
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
        for(QVector<QSharedPointer<Cuma_Main>>::iterator it = m_Send_Unit_list.begin(); it != m_Send_Unit_list.end(); it++)
        {
            //유닛들을 pop해서 해당 유닛의 핑을 체크함
            QSharedPointer<Cuma_Main> temp_unit;

            //유닛들을 먼저 pop함
            temp_unit = (*it)->f_pop_unit();

            //만약 유닛 리스트에 유닛들이 없을 경우 break으로 탈출
            if( temp_unit == nullptr)
            {
                break;
            }

            //유닛들의 딜레이 타임을 체크함
            QVector<uint32_t> get_unit_delay_array = m_Unit_delay_time_array[m_pid];

            uint32_t delay_time = get_unit_delay_array[temp_unit->mf_get_pid()];

            //딜레이 타임이 허용되는 기준에 초과되는지 체크함
            if(delay_time > limit_time)
            {
                //만약 초과된다면 m_Cuma_unit_list에 넣지 않음
                continue;
            }

            //전송할 유닛들 캐시 리스트에 넣음
            m_Send_Unit_list_cache.append(temp_unit);
        }

        //캐시 리스트들을 모든 유닛들의 행렬에 넣음
        m_Cuma_unit_list = m_Send_Unit_list_cache;

        //리턴함
        return 0;

    }catch(Cuma_Error& e)
    {
        e.show_error_string();
        return -1;
    }
}
