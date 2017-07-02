#ifndef CUMA_FILE_TEST_H
#define CUMA_FILE_TEST_H

#include <QObject>
#include <QDir>
#include <QTest>
#include <QFileInfoList>
#include <QFileInfo>
#include "../Cuma_Main_Unit/Cuma_File/cuma_file.h"
#include "cuma_file_test_env.h"

class Cuma_File_test : public Cuma_File
{
    Q_OBJECT
public:
    explicit Cuma_File_test(QObject *parent = 0);
    ~Cuma_File_test();

    //유닛 테스트 순서대로 정렬함
private slots:
    void t_set_File_Frag_Count();
    void t_set_File_Name();
    void t_set_File_info_block();
    
    void t_mf_Read_File();
    void t_mf_Read_File_Frag();
    void t_mf_Make_Frag();
    void t_mf_Save_by_Frag();
    void t_mf_Save_by_Frag_QVector();
    void t_mf_Save_by_File();
    
    void t_save_File_Frag_QVector();
    void t_read_File_Frag();
    void t_read_File();
    void t_clear_save_frag();
    void t_Save_frag_File();

protected:
    int set_env();
    QByteArray get_test_binary(QString f_name);
    QVector<QByteArray> get_test_frag_binary(QString f_name, uint32_t count);

    //test_dir만 남기고 전부 삭제
    int clear_test_env();


};

#endif // CUMA_FILE_TEST_H
