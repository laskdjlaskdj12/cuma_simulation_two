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

void Cuma_Main::set_pid(uint32_t pid)
{
    m_PId = pid;
}

uint32_t Cuma_Main::get_pid()
{
    return m_Pid;
}

QJsonObject Cuma_Main::mf_get_report_json()
{
    return m_report_json;
}

void Cuma_Main::sl_stop_unit()
{
    //전송 유닛리스트들이

}

void Cuma_Main::sl_recv_signal(QJsonObject o)
{

}

void Cuma_Main::sl_start_spread_signal(QString filename)
{

}

void Cuma_Main::f_recv_process(QJsonObject o)
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

int Cuma_Main::f_fragment_file(const QString file_name)
{

}

void Cuma_Main::f_push_unit(QSharedPointer<Cuma_Main> unit)
{

}

QSharedPointer<Cuma_Main> Cuma_Main::f_pop_unit()
{

}
