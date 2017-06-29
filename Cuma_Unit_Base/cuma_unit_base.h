#ifndef CUMA_UNIT_BASE_H
#define CUMA_UNIT_BASE_H

#include <QObject>
#include <QVector>
#include <QSharedPointer>
#include <QThread>
#include <QLIst>
#include "../Cuma_Main_Unit/cuma_main.h"


class Cuma_Unit_Base : public QObject
{
    Q_OBJECT
public:
    explicit Cuma_Unit_Base(QObject *parent = nullptr);
    ~Cuma_Unit_Base();
    
    void set_read_file_name(QString file_name);

    void Make_Unit_Random_Delay_time();
    void start_unit_spread();
    
signals:
    
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

    //읽을 파일 이름
    QString file_name;

    
};

#endif // CUMA_UNIT_BASE_H
