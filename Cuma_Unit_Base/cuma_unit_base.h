#ifndef CUMA_UNIT_BASE_H
#define CUMA_UNIT_BASE_H

#include <QObject>
#include <QVector>
#include <QSharedPointer>
#include <QThread>
#include <QLIst>
#include <QTime>
#include <cstdlib>
#include <QtGlobal>
#include <QStringList>
#include "../Cuma_Main_Unit/cuma_main.h"

#define TEST public



class Cuma_Unit_Base : public QObject
{
    Q_OBJECT
public:
    explicit Cuma_Unit_Base(QObject *parent = nullptr);
    ~Cuma_Unit_Base();
    
    void set_Cuma_Unit_Count(uint32_t count);
    void set_Cuma_Unit_Ping_Timeout(uint32_t timeout);
    int set_target_unit(uint32_t unit_pid);
    int set_target_to_read_File(QString file_name, QByteArray& arr);


    uint32_t get_Cuma_Unit_Count();
    QVector<QSharedPointer <Cuma_Main>> get_alloc_unit();
    QVector<QSharedPointer<QThread>> get_Cuma_Unit_Thread();
    QSharedPointer<Cuma_Main> get_target_unit();

    int Attach_Cuma_Unit_to_Thread();
    void Make_Unit_Random_Delay_time();    
    int init_Cuma_Unit(uint32_t count);
    int init_Cuma_Unit(uint32_t count, bool limit_timeout);
    void init_Cuma_Unit_File_Frag_dir();

    void remove_all_unit_file();
public:
    int start_unit_ping_spread();
    int start_all_unit_ping_spread();
    int start_unit_file_binary_spread();
    int start_unit_file_binary_request();
    int start_unit_file_binary_exsist();
    int start_unit_bypass();
public:
    void finish_thread();
TEST:
    uint32_t alloc_unit();
    uint32_t init_unit();
    uint32_t init_unit(bool time_out_limit);
    QVector<uint32_t> make_pid(uint32_t count);
    QVector<QVector<uint32_t>> make_Unit_Ping_Limit(uint32_t count, uint32_t limit);
    QMap <uint32_t, uint32_t> distribute_Ping_Timeout_Time(uint32_t pid_last_count, uint32_t timegap, bool is_distribitue);
    QMap <uint32_t, uint32_t> distribute_Ping_Timeout_Time(uint32_t pid_last_count, uint32_t timegap, bool is_distribitue, bool set_timeout );
signals:
    
    void start_command(QJsonObject o);

    //일시 중단할때 사용함
    void stop();
    
public slots:
    
private:

    //쓰레드 갯수
    uint32_t core_count;
    
    //쓰레드 풀
    QList<QSharedPointer<QThread>> Thread_Pool;
    
    //유닛들의 갯수
    uint32_t unit_count;
    
    //유닛들 리스트
    QVector<QSharedPointer<Cuma_Main>> Cuma_unit_list;

    QString file_name;

    uint32_t unit_timeout;

    QVector<QSharedPointer<QThread>> Cuma_Thread_list;
    
    QSharedPointer<Cuma_Main> target_Cuma_Unit;
};

#endif // CUMA_UNIT_BASE_H
