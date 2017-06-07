#include "cuma_main.h"

Cuma_Main::Cuma_Main(QObject *parent) : QObject(parent),
    unit_count(0), 
    core_count(0),
    is_ping_message(false)
{

}

Cuma_Main::~Cuma_Main()
{

}

void Cuma_Main::start_event_loop()
{
    //모든 유닛들에게 핑을 보냄

    //리스트 안에 있는 타겟 유닛들중 하나를 설정함

    //타이머를 시작후를 안에 넣고 맵핑함

    //해당 유닛에게 ping 메세지를 전송
}

int Cuma_Main::unit_event_loop()
{
    //connect는 unit_event_loop에서 타겟 라이브러리에 connect함

    //만약 핑요청 프로토콜일경우
    // 해당 유닛id를 찾아서 핑타임의 sleep을 하고 전송
    // 자신의 send_signal이 유닛에게 connect를 하고 전송

    //만약 파일 확인 프로토콜 일경우

    //만약 파일 전송 프로토콜 일 경우

    //만약 팡리 읽기 프로토콜 일경우


}

