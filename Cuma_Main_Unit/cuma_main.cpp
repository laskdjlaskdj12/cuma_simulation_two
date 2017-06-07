#include "cuma_main.h"

Cuma_Main::Cuma_Main(QObject *parent) : QObject(parent),
    unit_count(0),
    core_count(0),
    is_ping_message(false)
{

    //recv 시그널을 바인딩함
    connect(this, SIGNAL(recv_message_si(uint32_t,QJsonObject)), this, SLOT(recv_message_sl(uint32_t,QJsonObject)));
}

Cuma_Main::~Cuma_Main()
{
    disconnect(this, SIGNAL(recv_message_si(uint32_t,QJsonObject)), this, SLOT(recv_message_sl(uint32_t,QJsonObject)));
}

void Cuma_Main::start_event_loop()
{
    //모든 유닛들에게 핑을 보냄

    //리스트 안에 있는 타겟 유닛들중 하나를 설정함
    foreach (QVector<QSharedPointer<Cuma_Main>>::iterator iter, target_array_list) {

        //시간을 잴 타이머를 세팅함
        QTime unit_ping_return_time;

        //타임을 start함
        unit_ping_return_time.start();

        //타이머를 시작후를 안에 넣고 맵핑함
        measure_ping_unit_time[(*(*iter))->get_unit_pid()] = unit_ping_return_time;

        //해당 유닛에게 ping 메세지를 전송
        emit (*(*iter))->recv_message_si( cuma_protocol::ping_protocol(unit_pid, true));
    }


}

int Cuma_Main::unit_event_loop(QJsonObject obj)
{
    //만약 핑요청 프로토콜일경우
    if(obj["request"].toString() == "PIng"){

        //만약 핑요청한 프로토콜이 되돌아 올경우
        if(obj["responce"].toBool() == true){

            //핑을 요청한 프로토콜의 pid를 토대로 maping되어있는 QTime을 찾음
            uint32_t req_pid = get_pid_from_json(obj);

            //해당 타임을 elapsed()를 통해서 리턴된 milisec를 받음
            int time =  measure_ping_unit_time[req_pid].elapsed();

            //
        }
        // 해당 유닛id를 찾아서 핑타임의 sleep을 하고 전송

        //자신의 pid를 중심으로 구성된 유닛들 타임 딜레이리스트 를 찾음
        QVector<uint32_t> mypid_vector = time_array[unit_pid];

        //유닛들의 타임 딜레이 리스트 중 수신된 유닛의 pid의 타임 딜레이를 찾음;
        uint32_t time_delay = mypid_vector[static_cast<unsigned int>(obj["pid"].toInt())];

        //해당 타임 딜레이 만큼 sleep함
        QThread::msleep(time_delay);

        //해당 유닛을 타겟으로 하여 전송함



    }
    // 자신의 send_signal이 유닛에게 connect를 하고 전송

    //만약 파일 확인 프로토콜 일경우

    //만약 파일 전송 프로토콜 일 경우

    //만약 팡리 읽기 프로토콜 일경우


}

uint32_t Cuma_Main::get_pid_from_json(QJsonObject obj)
{
    return static_cast<uint32_t>(obj["pid"].toInt());
}

