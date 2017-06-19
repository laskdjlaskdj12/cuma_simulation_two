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
            emit unit->s_recv(cuma_protocol::ping_protocol(m_Pid));
        }

        //저장되어있는 유닛리스트의 delay time을 체크함
        f_send_ping_to_unit(m_ping_limit);

        //파일의 바이너리를 읽음
        if (f_f_read_file(file_name) < 0)
        {
            //파일바이너리 읽기가 실패할 경우 파일 에러 메세지를 읽어서 throw함
            throw Cuma_Debug("Error of file_binary_read :" + m_File->mf_get_file_error_string() + " Unit_Id : " + m_Pid, __LINE__);
        }

        //파일 바이너리를 파편화 시킴
        if(f_fragment_file(file_))

    }
    catch(Cuma_Debug& debug)
    {

    }

}

void Cuma_Main::f_recv_process(const QJsonObject& o)
{

}

void Cuma_Main::f_write_report_json(QJsonObject o)
{

}

QTime Cuma_Main::f_tell_time()
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

}

int Cuma_Main::f_send_ping_to_unit(uint32_t limit_time)
{
    //디버그 메세지
    Cuma_Debug("Send Ping protocol to unit to unit PID: " +  m_Pid);

    //모든유닛에게 핑 프로토콜을 전송함
    for(QVector<QSharedPointer<Cuma_Main>>::iterator it = m_Cuma_unit_list.begin(); it != m_Cuma_unit_list.end(); it++)
    {
        //emit으로 유닛들에게 시그널을 전송함
        emit (*it)->s_recv(cuma_protocol::ping_protocol(m_Pid));
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

}
