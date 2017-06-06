#include "cuma_main.h"

Cuma_Main::Cuma_Main(QObject *parent) : QObject(parent),
    unit_count(0), 
    core_count(0)
{
    connect(this, SIGNAL(send_ping_message_si_out(QByteArray)), this, SLOT(send_file_message_sl_out(QByteArray)));
    connect(this, SIGNAL(recv_ping_message_si_in(QByteArray)), this, SLOT(send_file_message_sl_out(QByteArray)));
    connect(this, SIGNAL(send_file_message_si_out(QByteArray)), this, SLOT(send_file_message_sl_out(QByteArray)));
    connect(this, SIGNAL(recv_file_message_si_in(QByteArray)), this, SLOT(send_file_message_sl_out(QByteArray)));

}

Cuma_Main::~Cuma_Main()
{

}

void Cuma_Main::read_file_name(QString f)
{

}

void Cuma_Main::set_timeout(uint32_t timeout)
{

}

void Cuma_Main::copy_unit_list(QList<QSharedPointer<Cuma_Main> > list)
{

}

QJsonObject Cuma_Main::get_result_json()
{

}

void Cuma_Main::send_ping_message_sl()
{

}

void Cuma_Main::recv_ping_message_sl()
{

}

void Cuma_Main::send_file_message_sl_out(QByteArray binary)
{
    /*
     * 1. 타겟의 recv_file_message_si_in()의 시그널을 emit함
     * 2. 타겟이 자신의 recv_file_message_si_in()으로 호출의 slot을 확인함
     * 3. 자신의 json에 해당 타겟의 pid와 보낸 프래그먼트와 리턴 타임을 작성함
     * */
}

void Cuma_Main::recv_file_message_sl_in(QByteArray binary)
{

}

void Cuma_Main::stop_simulation_sl()
{

}
