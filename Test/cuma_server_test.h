#ifndef CUMA_SERVER_TEST_H
#define CUMA_SERVER_TEST_H

#include <QObject>
#include <QVector>
#include <QTest>
#include <stdarg.h>
#include <QSignalSpy>
#include "../Cuma_Main_Unit/cuma_main.h"

class Cuma_server_test : public Cuma_Main
{
    Q_OBJECT
public:
    explicit Cuma_server_test(QObject *parent = nullptr);
    ~Cuma_server_test();

private slots:
    void t_f_upload_file_frag_from_unit();
    void t_f_reply_upload_file_frag_to_unit();

    void t_f_download_file_frag_from_unit();
    void t_f_reply_download_file_frag_to_unit();

    void t_f_check_file_frag_to_unit();
    void t_f_reply_check_file_frag_to_unit();

    void t_f_over_bypass();
    void t_f_reply_over_bypass_limit();

private:
    int env_make_File(QString f_name);
    int env_make_Frag(QString f_name, uint32_t count);

    QByteArray env_get_file_binary(QString f_name);

    void init_unit(uint32_t count, QVector<QSharedPointer<Cuma_Main>>& unit_vec);
    void env_set_env(uint32_t count, QSharedPointer<Cuma_Main> a, QSharedPointer<Cuma_Main> b);
    void env_clear_env();
    void env_clear_binary();

private:
    QString root_path;


};

class Cuma_serv_test_unit : public Cuma_Main
{
    Q_OBJECT
public:
    Cuma_serv_test_unit();
    ~Cuma_serv_test_unit();

    void read_file();
    void read_file(QString f_name);
    void set_file_name(QString f_name);

    void write_file();
    void write_file(QString f_name, QByteArray& arr);
    void write_file_frag(QString f_name, QByteArray& arr, uint32_t index);
    void env_clear_test_unit_env();

};

#endif // CUMA_SERVER_TEST_H
