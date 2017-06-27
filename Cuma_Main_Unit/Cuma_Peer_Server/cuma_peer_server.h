#ifndef CUMA_PEER_SERVER_H
#define CUMA_PEER_SERVER_H

#include <QObject>
#include "../cuma_main.h"
using Cuma_Main;

class Cuma_Peer_Server : public QObject
{
    Q_OBJECT

    //서버 Area
protected:
    // 파일 업로드 프로세스
    // (유닛으로부터 파일 frag 업로드 명령이 왔을시)
    virtual int f_upload_file_frag_from_unit(QJsonObject o);
    virtual int f_reply_upload_file_frag_to_unit(QJsonObject o);

    // 파일 다운로드 프로세스
    // (유닛으로부터 파일 frag 를 저장 하라는 명령이 왔을시)
    virtual int f_download_file_frag_from_unit(QJsonObject o);
    virtual int f_reply_download_file_frag_to_unit(QJsonObject o);

    // 파일 체크 프로세스
    // (바이패스를 같이 포함함)
    virtual int f_check_file_frag_to_unit(QJsonObject o);
    virtual int f_reply_check_file_frag_to_unit(const QJsonObject o);

    //바이패스 프로세스
    // (모든 유닛에게 받은 QJsonObject를 전송하는 역활)
    virtual int f_over_bypass(QJsonObject o);
    //바이패스  초과 프로세스
    // (유닛에게 리플라이가 오는 역활을 함)
    virtual int f_reply_over_bypass_limit(QJsonObject o);

public:
    friend class Cuma_Main;

};

#endif // CUMA_PEER_SERVER_H
