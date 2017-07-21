#ifndef CUMA_CLIENT_TEST_H
#define CUMA_CLIENT_TEST_H

#include <QObject>
#include <QSignalSpy>
#include <QTest>
#include "../Cuma_Main_Unit/cuma_main.h"

class Cuma_client_test : public Cuma_Main
{
    Q_OBJECT
public:
    explicit Cuma_client_test(QObject *parent = nullptr);
    ~Cuma_client_test();
private slots:
    /*void t_mf_command_set_file_name();
    void t_mf_command_set_unit_bypass_count();
    void t_mf_command_ping_test();*/
    void t_mf_command_spread_test();
    /*void t_mf_command_req_file_test();
    void t_mf_command_trace_pass_test();*/

protected:
    int env_set_test_env_unit(QVector<QSharedPointer<Cuma_Main>>& v, uint32_t max_timeout);
    void env_clear_env();
    void env_clear_cache();
    int env_make_file(QString f_name);
    void env_remove_file(QString f_name);

    QByteArray env_get_file_binary(QString f_name);
    QVector<QByteArray> env_get_file_frag(QString f_name, uint32_t count);
    QVector<QSharedPointer<Cuma_Main>> env_make_unit(uint32_t count);
    QVector<uint32_t> env_allow_ping_unit(uint32_t limit, QVector<uint32_t> delay_list, uint32_t pid);
    bool env_find_pid(uint32_t i, QVector<QSharedPointer<Cuma_Main>>& v);
    bool env_compare_file_frag(QVector<QByteArray> test_unit_frag_list, QVector<QByteArray> frag_list);

private:
    QString root_path;

};


#endif // CUMA_CLIENT_TEST_H
