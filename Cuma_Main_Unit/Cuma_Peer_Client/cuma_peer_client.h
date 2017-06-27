#ifndef CUMA_PEER_CLIENT_H
#define CUMA_PEER_CLIENT_H

#include <QObject>
#include "../cuma_main.h"
using Cuma_Main;

class Cuma_Peer_Client : public QObject
{
    Q_OBJECT

    //유닛을 제어하는 노드에서 커맨드 함수
    //Client 영역
protected:
    //유닛이 읽을 파일 이름
    virtual int mf_command_set_file_name(QString f_name);

    //유닛의 바이패스 횟수를 카운트
    virtual void mf_command_set_unit_bypass_count(uint32_t count);

    //이 유닛이 ping을 메인 유닛
    virtual int mf_command_ping_test();

    //이 유닛이 spread의 메인 유닛
    virtual int mf_command_spread_test();

    //이 유닛이 파일 요청의 메인 유닛
    virtual int mf_command_req_file_test();

    //이 유닛이 파일 track의 메인 유닛 (반드시 자기자신이 bypass_limit_count이고 프로토콜 전송하기전에 -1를 할것)
    virtual int mf_command_trace_pass_test();

    friend class Cuma_Main;
};

#endif // CUMA_PEER_CLIENT_H
