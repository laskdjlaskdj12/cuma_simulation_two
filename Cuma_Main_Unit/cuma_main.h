#ifndef CUMA_MAIN_H
#define CUMA_MAIN_H

#include <QObject>
#include <QVector>
#include <QTime>
#include <QTimer>
#include <QThread>
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

    //전송된 유닛들의 시간들의 리포트를 json으로 리턴함
    QJsonObject get_result_json();

    //유닛 타이밍
    uint32_t get_unit_timer();

    //핑 메세지
signals:

    /*
     * 중요 : 반드시 send, recv의 시그널들은 네트워크 소켓을 본따 만들어졌음
     *  ex) send -> (자기 유닛을 중심으로) 자기가 다른 유닛에게 전달할때
     *  자기 유닛 안에 있는 send_out slot을 emit해서 바이너리를 전달하는 방식 (즉 스스로 send를 emit함)
     *  그리고 받는 유닛에서는 recv_in의 시그널을 받아서 connect된 slot을 작동하게 하는 방식
     *
     * signal, slog을 각 객체간의 메세지처럼 이용
     *
     * 1. 자기를 기준으로 다른 유닛에게 전송하는것을 out으로 함
     * 2. 다른 유닛이 자기 시그널을 emit하는것을 in으로함
     * */

    //메세지 프로시저
    void recv_message_si (QJsonObject obj);

    //attach된 쓰레드에 끝났다는것을 리턴함
    void quit_si_int_out();

    //메인에서 stop시그널이 왔을경우
    void stop_si_in();
 
    //내부에서 메세지 프로시저
protected slots:
    void recv_message_sl (QJsonObject obj);

public slots:
    //메인에서 stop시그널이 왔을경우
    void stop_simulation_sl();

    //시뮬레이션을 시작함
    void start_event_loop();

protected:
    //내부 이벤트 루프문
    int unit_event_loop(QJsonObject);

    //Jsonobject문에 있는 pid를 추출함
    uint32_t get_pid_from_json(QJsonObject obj);

private:

    //유닛의 pid
    uint32_t  unit_pid;

    QList<QByteArray> file_frag;
    
    //Cuma_Main의 타겟설정
    QSharedPointer<Cuma_Main> target_unit;

    //리턴타임의 핑타임의 행렬을 구성함
    QVector<QVector<uint32_t> > time_array;
    
    //핑이 타임아웃내에 있는 어레이 리스트
    QVector<QSharedPointer<Cuma_Main>> target_array_list;

    //핑타임 재는 타이머
    QTime p_time;

    //타임아웃 타이머
    QTimer timeout_timer;

    //타임아웃 시간
    uint32_t timeout_time;

    //핑 메세지가 전송되었는지 확인
    bool is_ping_message;

    QMap<uint32_t, QTime> measure_ping_unit_time;


};

#endif // CUMA_MAIN_H

class cuma_protocol{
    static QJsonObject ping_protocol(uint32_t unit_id, bool is_return);
    static QJsonObject is_file_exsist_protocol(uint32_t file_frag_index, uint32_t unit_id);
    static QJsonObject file_binary_save_protocol(QJsonObject file_binary, uint32_t unit_id);
    static QJsonObject file_binary_read_protocol(uint32_t file_frag_index, uint32_t );

};
