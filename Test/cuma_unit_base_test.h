#ifndef CUMA_UNIT_BASE_TEST_H
#define CUMA_UNIT_BASE_TEST_H

#include <QObject>
#include <QThread>
#include <QTest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <Cuma_Unit_Base/cuma_unit_base.h>

class Cuma_Unit_Base_Test : public QObject
{
    Q_OBJECT
public:
    explicit Cuma_Unit_Base_Test(QObject *parent = nullptr);
    ~Cuma_Unit_Base_Test();

private slots:
    /*void testSetUnitCount();
    void testAllocUnit();
    void testMakeUnitPid();
    void testMakeUnitPingLimit();
    void testDistributePingTimeoutTime();
    void testSetinitUnit();
    void testSetUnitAttachThread();
    void start_all_unit_ping_test();
    void start_unit_file_binary_spread_test();
    void testUnitRemoveAllFile();
    void start_unit_file_binary_exsist_test();
    void start_unit_file_binary_request_test();*/
    void testBypassUnit();
    void testBypassPayload();
    //void start_unit_bypass_test();
};

#endif // CUMA_UNIT_BASE_TEST_H
