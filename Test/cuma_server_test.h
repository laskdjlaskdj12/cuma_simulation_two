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

signals:

public slots:
};

#endif // CUMA_SERVER_TEST_H
