#ifndef CUMA_TEST_ENV_H
#define CUMA_TEST_ENV_H

#include <QObject>
#include <QVector>
#include <QSharedPointer>
#include "../Cuma_Main_Unit/cuma_main.h"

class C_M_Test_inv:public Cuma_Main
{
    Q_OBJECT
public:
    explicit C_M_Test_inv(QObject* parent = 0);

    Cuma_Main get_Test_Env();

    void add_pid(uint32_t i, uint32_t limit_time);
    void add_ptr(QSharedPointer<Cuma_Main>& p);
    void add_pid_vec(QVector<QVector<uint32_t>>& v);
    void add_ptr_vec(QVector<QSharedPointer<Cuma_Main>>& v);

    void remove_pid(uint32_t i);
    void remove_ptr(QSharedPointer<Cuma_Main>& p);
    void remove_pid_vec(QVector<uint32_t>& v);
    void remove_ptr_vec(QVector<QSharedPointer<Cuma_Main>>& v);

};

#endif // CUMA_TEST_ENV_H
