#ifndef CUMA_FILE_TEST_H
#define CUMA_FILE_TEST_H

#include <QObject>
#include <QDir>
#include <QTest>
#include <QFileInfoList>
#include <QFileInfo>
#include "../Cuma_Main_Unit/Cuma_File/cuma_file.h"

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

    int env_set_env();
    QByteArray env_get_test_binary(QString f_name);
    QVector<QByteArray> env_get_test_frag_binary(QString f_name, uint32_t count);

    //test_dir만 남기고 전부 삭제
    int env_clear_test_env();

    //테스트 영역에 파일을 생성하는 함수
    int env_mk_file(QByteArray& arr, QString f_name);
    int env_mk_file(QString f_name);

private:
    QString root_path;

};

#endif // CUMA_FILE_TEST_H
