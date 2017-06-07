#include "cuma_main.h"

Cuma_Main::Cuma_Main(QObject *parent) : QObject(parent),
    unit_count(0), 
    core_count(0),
    is_ping_message(false)
{
    connect(this, SIGNAL(send_ping_message_si_out(QByteArray)), this, SLOT(send_ping_message_sl_out(QByteArray)));
    connect(this, SIGNAL(recv_ping_message_si_in(QSharedPointer<Cuma_Main>), this, SLOT(recv_ping_message_sl_in(QSharedPointer<Cuma_Main>)));
    connect(this, SIGNAL(send_file_message_si_out(QByteArray)), this, SLOT(send_file_message_sl_out(QByteArray)));
    connect(this, SIGNAL(recv_file_message_si_in(QByteArray)), this, SLOT(recv_file_message_sl_in(QByteArray)));

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
    //핑이 전송됨을 나타내는 플래그
    is_ping_message = true;
    emit target_unit->recv_ping_message_sl_in(this);
}

void Cuma_Main::recv_ping_message_sl(QSharedPointer<Cuma_Main> t)
{
    //만약 핑이 왔을 경우
    if(is_ping_message == true){


    }
    //해당 타겟의 sharedpointer를
    //타겟의 delay 타임을 분석한후 전송
   QVector<unit32_t> my_ping_array =   target_array_list[unit_pid];
   uint32_t delay_timer = my_ping_array[t->get_unit_timer()];

   //해당 타이밍까지 wait함
    QThread::sleep(delay_timer);

    //타이밍이 끝나면 해당 유닛의 시그널로 emit함
    target_unit = t;
    emit target_unit->send_ping_message_si_out();
}

void Cuma_Main::send_file_message_sl_out(QByteArray binary)
{
    /*
     * 1. 타겟의 recv_file_message_si_in()의 시그널을 emit함
     * 2. 타겟이 자신의 recv_file_message_si_in()으로 호출의 slot을 확인함
     * 3. 자신의 json에 해당 타겟의 pid와 보낸 프래그먼트와 리턴 타임을 작성함
     * */

    emit target_unit->recv_file_message_sl_in(binary);

}

void Cuma_Main::recv_file_message_sl_in(QByteArray binary)
{
    QJsonDocument j_doc;
    QJsonObject j_obj;

   j_doc =  QJsonDocument::fromBinaryData(binary);

   //obj으로 변환함
   j_obj = j_doc.object();

}

void Cuma_Main::stop_simulation_sl()
{

}
