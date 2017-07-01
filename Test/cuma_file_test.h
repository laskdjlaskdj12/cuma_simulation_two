#ifndef CUMA_FILE_TEST_H
#define CUMA_FILE_TEST_H

#include <QObject>
#include <QTest>
#include "../Cuma_Main_Unit/Cuma_File/cuma_file.h"

class Cuma_File_test : public Cuma_File
{
    Q_OBJECT
public:
    explicit Cuma_File_test(QObject *parent = 0);
    ~Cuma_File_test();

private slots:

};

#endif // CUMA_FILE_TEST_H
