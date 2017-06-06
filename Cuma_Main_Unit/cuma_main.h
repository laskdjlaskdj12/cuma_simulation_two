#ifndef CUMA_MAIN_H
#define CUMA_MAIN_H

#include <QObject>
#include <QVector>
#include <QTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSharedPointer>
#include "Cuma_File/cuma_file.h"


class Cuma_Main : public QObject
{
    Q_OBJECT
public:
    explicit Cuma_Main(QObject *parent = nullptr);
    ~Cuma_Main();
    
    //파일을 전송할 파일 이름
    void read_file_name(QString f);
    
    //타임아웃을 세팅함
    void set_timeout(uint32_t timeout);

    //유닛 리스트들을 복사함
    void copy_unit_list(QList<QSharedPointer<Cuma_Main>> list);

    QJsonObject get_result_json();

    //핑 메세지
signals:
    void send_ping_message_si();
    void recv_ping_message_si();

    //파일 메세지
    void send_file_message_si(QByteArray);
    void recv_file_message_si(QByteArray);

    //메인에서 강제로 종료했을시 리턴함
    void quit_si();
 
    //내부에서 메세지 프로시저
public slots:
    void send_ping_message_sl();
    void recv_ping_message_sl();

    void send_file_message_sl(QByteArray binary);
    void recv_file_message_sl(QByteArray binary);

    //쓰레드가 끝났다는것을 리턴함
    void stop_simulation();

private:
    
    //파일 read
    bool read_file();
    
private:
    
    //Cuma_Main의 타겟설정
    QSharedPointer<Cuma_Main> target;

    //리턴타임의 핑타임의 행렬을 구성함
    QVector<QVector<uint32_t> > time_array;
    
    //핑이 타임아웃내에 있는 어레이 리스트
    QVector<QSharedPointer<Cuma_Main>> target_array_list;

    //핑타임 재는 타이머
    QTime clock;

    //타임아웃 타이머
    QTime timeout_timer;

    //타임아웃 시간
    uint32_t timeout_time;

};

#endif // CUMA_MAIN_H
