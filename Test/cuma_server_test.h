#ifndef CUMA_SERVER_TEST_H
#define CUMA_SERVER_TEST_H

#include <QObject>
#include <QTest>
#include "../Cuma_Main_Unit/cuma_main.h"

class Cuma_server_test : public Cuma_Main
{
    Q_OBJECT
public:
    explicit Cuma_server_test(QObject *parent = nullptr);

private slots:
    void t_f_upload_file_frag_from_unit();
    void t_f_reply_upload_file_frag_to_unit();

    void t_f_download_file_frag_from_unit();
    void t_f_reply_download_file_frag_to_unit();

    void t_f_check_file_frag_to_unit();
    void t_f_reply_check_file_frag_to_unit();

    void t_f_over_bypass();
    void t_f_reply_over_bypass_limit();
};

#endif // CUMA_SERVER_TEST_H
