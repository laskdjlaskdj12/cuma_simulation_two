#ifndef CUMA_MAIN_UNIT_TEST_H
#define CUMA_MAIN_UNIT_TEST_H

#include <random>
#include <QVector>
#include <QObject>
#include <QSignalSpy>
#include <QSharedPointer>
#include <QtTest/qtest.h>

#include "../Cuma_Main_Unit/Cuma_Peer_Protocol/cuma_peer_protocol.h"
#include "../Cuma_Main_Unit/cuma_main.h"

#define Cuma_Debug_str(X) Cuma_Debug(X, __LINE__)

class Cuma_Main_Unit_test : public Cuma_Main
{
    Q_OBJECT
public:
    explicit Cuma_Main_Unit_test(QObject *parent = 0);

private slots:
    void t_set_ulist_property();
    void t_set_ping_limit_property();
    void t_is_active_property();

    //signal
    void t_s_stop_unit();
    void t_s_recv();
    void t_s_start_command();

    //유닛들의 툴
    void t_f_recv_process();
    void t_f_tell_time();
    void t_f_find_unit_from_inside_timeout_unit();
    void t_f_find_unit_from_Cuma_unit_list();

    //유닛 리스트 저장
    void t_f_push_unit();
    void t_f_pop_unit();
    void t_f_f_save_recv_json_report();

protected:
    void env_clear_cache();
    my_ping_list env_make_ping_list(uint32_t count);

    //pid를 주면 해당 pid의 행렬에 해당하는 pid_ping_lst를 돌려받음
    my_ping_list env_make_by_unit_ping_list(uint32_t pid, uint32_t count);
    another_pid_ping_array env_make_by_unit_ping_array(uint32_t count);
};

#endif // CUMA_MAIN_UNIT_TEST_H
