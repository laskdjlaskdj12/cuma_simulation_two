#include "cuma_main.h"

Cuma_Main::Cuma_Main(QObject *parent) : QObject(parent),
    unit_count(0), 
    core_count(0)
{
    
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

}

void Cuma_Main::recv_file_message_sl_in(QByteArray binary)
{

}

void Cuma_Main::stop_simulation_sl()
{

}
