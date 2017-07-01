#ifndef CUMA_MAIN_UNIT_TEST_H
#define CUMA_MAIN_UNIT_TEST_H

#include <QObject>
#include <QtTest/qtest.h>
#include "../Cuma_Main_Unit/cuma_main.h"

class Cuma_Main_Unit_test : public Cuma_Main
{
    Q_OBJECT
public:
    explicit Cuma_Main_Unit_test(QObject *parent = 0);

signals:

public slots:
};

#endif // CUMA_MAIN_UNIT_TEST_H
