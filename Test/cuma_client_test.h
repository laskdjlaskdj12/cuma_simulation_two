#ifndef CUMA_CLIENT_TEST_H
#define CUMA_CLIENT_TEST_H

#include <QObject>
#include <QTest>
#include "../Cuma_Main_Unit/cuma_main.h"

class Cuma_client_test : public Cuma_Main
{
    Q_OBJECT
public:
    explicit Cuma_client_test(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CUMA_CLIENT_TEST_H
