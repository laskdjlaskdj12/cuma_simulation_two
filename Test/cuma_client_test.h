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
    void t_mf_command_set_file_name();
    void t_mf_command_set_unit_bypass_count();
    void t_mf_command_ping_test();
    void t_mf_command_spread_test();
    void t_mf_command_req_file_test();
    void t_mf_command_trace_pass_test();

protected:
    int env_set_test_env_unit();
    void env_clear_cache();
    int env_make_file(QString f_name);
    void env_remove_file(QString f_name);

private:
    QString root_path;

};

class Test_Unit: public Cuma_Main
{
    Q_OBJECT
public:
     Test_Unit();
    ~Test_Unit();

    void set_read_file_name(QString f_name);
    void read_file();
    void read_file(QString f_name);

    int write_file();
    int write_file(QString f_name, QByteArray& arr);

    Cuma_File get_file_obj();


};

#endif // CUMA_CLIENT_TEST_H
