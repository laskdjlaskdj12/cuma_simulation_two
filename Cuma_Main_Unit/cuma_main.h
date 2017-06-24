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
#include <QMutexLocker>
#include "Cuma_File/cuma_file.h"
#include "../Cuma_Debug/cuma_debug.h"

class Cuma_Unit :public QObject
{
    Q_OBJECT
public:
    explicit Cuma_Unit(QObject* obj = nullptr);

    //인터페이스 영역
    virtual void set_unit_array(QVector<Cuma_Unit> unit_array) = 0;
    virtual QVector<Cuma_Unit> get_unit_array() = 0;
    virtual void set_unit_id(uint32_t) = 0;
    virtual uint32_t get_unit_id() = 0;

    virtual uint32_t start_spread(QString file_name) = 0;

signals:
    void recv_Unit(QByteArray array);

protected:

private:
    //디폴트 대입생성자 는 금함
    void operator = (Cuma_Unit& u) = 0;
};

class Cuma_Main : public QObject
{
    Q_OBJECT
public:

    explicit Cuma_Main(QObject* parent = null);
    ~Cuma_Main();

    //unit 리스트 프로퍼티
    void mf_set_unit_list(QVector<Cuma_Main> list);
    QVector<Cuma_Main> mf_get_unit_list();

    //ping 리스트 리미트 프로퍼티
    void mf_set_ping_limit_time(uint32_t time);
    uint32_t mf_get_ping_limit_time();

    void mf_set_pid(uint32_t pid);
    uint32_t mf_get_pid();

    //json 리포트 받기
    QJsonObject mf_get_report_json();

    //active되었는지 확인
    bool mf_is_active();
    void mf_set_active(bool);

    //유닛을 제어하는 노드에서 커맨드 함수
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

    //이 유닛이 파일 track의 메인 유닛
    virtual int mf_command_trace_pass_test();


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
    
    //쓰레드가 attach가 된 상태에서 컨트롤 서버에서 본 유닛에게 stop 요청 시그널
    void s_stop_unit();
    
    //다른 유닛에서 recv 요청 시그널
    void s_recv(QJsonObject o);
    
    //메인 컨트롤에서 파일을 읽고 다른유닛에게 전파 명령 시그널
    void s_start_command(const QJsonObject command);

protected slots:
    //s_stop_unit()의 slot
    void sl_stop_unit();

    //s_recv()의 slot
    void sl_recv_signal(QJsonObject o);

    //s_start_spread()의 slot
    void sl_start_command_signal(const QJsonObject command);
    
protected:

    //recv의 수신 프로세스
    virtual void f_recv_process(const QJsonObject& o);

    //현재 타이머의 milisec를 알려주는 프로세스
    virtual int f_tell_time();

protected:
    //전송할 유닛 리스트들을 push하는 프로세스
    virtual void f_push_unit(QSharedPointer<Cuma_Main> unit);

    //전송을 시작할 유닛 리스트들을 pop하는 프로세스
    virtual QSharedPointer<Cuma_Main> f_pop_unit();

    //모든 유닛들의 핑을 전송해서 리턴된 핑의 딜레이 탐색 프로세스
    virtual int f_send_ping_to_unit(uint32_t limit_time);

    //수신 json로그 저장 프로세스
    virtual void f_save_recv_json_report(QJsonValue protocol);

    //송신 json로그 저장 프로세스
    virtual void f_save_send_json_report(QJsonValue protocol);


    //서버 Area
protected:
    // 파일 업로드 프로세스
    // (유닛으로부터 파일 frag 업로드 명령이 왔을시)
    virtual int f_upload_file_frag_to_unit(QJsonObject& o);
    virtual int f_reply_upload_file_frag_to_unit(QJsonObject& o);

    // 파일 다운로드 프로세스
    // (유닛으로부터 파일 frag 를 저장 하라는 명령이 왔을시)
    virtual int f_download_file_frag_to_unit(QJsonObject& o);
    virtual int f_reply_download_file_frag_to_unit(QJsonObject& o);

    // 파일 체크 프로세스
    // (바이패스를 같이 포함함)
    virtual int f_check_file_frag_to_unit(QJsonObject& o);
    virtual int f_reply_check_file_frag_to_unit(const QJsonObject& o);

    //바이패스 프로세스
    // (모든 유닛에게 받은 QJsonObject를 전송하는 역활)
    virtual int f_over_bypass(QJsonObject& o);
    //바이패스  초과 프로세스
    // (유닛에게 리플라이가 오는 역활을 함)
    virtual int f_reply_over_bypass_limit(QJsonObject& o);

private:
    //모든유닛들의 delay_time 행렬
    QVector<QVector<uint32_t>> m_Unit_delay_time_array;

    //모든 유닛들의 행렬
    QVector<QSharedPointer<Cuma_Main>> m_Cuma_unit_list;

    //핑 타임아웃내의 유닛들의 행렬
    QVector<QSharedPointer<Cuma_Main>> m_Cuma_unit_inside_timeout_unit_list;

    //전송할 유닛들 리스트 (유닛당 전송이 완료가 되면 자동으로 pop함)
    QVector<QSharedPointer<Cuma_Main>> m_Send_Unit_list;

    //Cuma_File 애트리뷰트
    QSharedPointer<Cuma_File> m_File;

    //현재 유닛 pid
    uint32_t m_Pid;

    //현재 유닛의 리미트 ping 타임
    uint32_t m_ping_limit;

    //유닛의 리포트 json
    QJsonObject m_report_json;

    //유닛리스트를 push할때 뮤텍스 locker
    QMutex m_locker;

    //이 유닛이 active했는지 확인함
    bool m_active;

    //유닛의 바이패스 리미트 카운트를 잼
    static uint32_t m_limit_bypass_count;

};
class unit_Timer{
public:

    //타이머 본체
    static QTime time;

    //타이머가 시작됬는지
    static bool is_start;
};

class cuma_protocol{

public:
    static QJsonObject req_unit_command_protocol(QString command);
    static QJsonObject req_unit_command_protocol(QString command, uint32_t count);
    static QJsonObject req_unit_command_protocol(QString command, QString name);

public:
    static QJsonObject req_ping_protocol(uint32_t unit_id, bool is_return = false);
    static QJsonObject req_is_file_exsist_protocol(uint32_t file_frag_index, uint32_t unit_id);
    static QJsonObject req_file_binary_save_protocol(QJsonObject file_binary, uint32_t unit_id);
    static QJsonObject req_file_binary_read_protocol(QString binary_name, uint32_t file_frag_index, uint32_t unit_id );

public:
    static QJsonObject reply_ping_protocol(uint32_t unit_id, bool is_return = false);
    static QJsonObject reply_is_file_exsist_protocol(uint32_t file_frag_index, uint32_t unit_id, bool is_exsist);
    static QJsonObject reply_file_binary_save_protocol(QString file_frag_name, uint32_t unit_id, uint32_t frag_index);
    static QJsonObject reply_file_binary_read_protocol(QString file_frag_name, uint32_t file_frag_index, uint32_t unit_id, QByteArray binary );

};

#endif // CUMA_MAIN_H
