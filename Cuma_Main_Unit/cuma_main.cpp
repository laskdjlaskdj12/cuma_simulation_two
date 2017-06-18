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

        //while문으로 유닛리스트들에 유닛들이 있는지 검사해서 있으면 검사하고 전송함
        while()
        {
            //자기 자신 인덱스를 제외한 모든 유닛들의 인덱스를 확인함

        }
        //파일의 바이너리를
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
