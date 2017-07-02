#include "cuma_file_test.h"

Cuma_File_test::Cuma_File_test (QObject *parent)
{
    Cuma_Debug::show_debug(true);

    //test할 디렉토리를 삭제하고 새로 생성함
    QDir mktest;
    if(mktest.exists("t_unit_dir") == true)
    {
        mktest.remove("t_unit_dir");
    }

    mktest.mkdir("t_unit_dir");

    //현재 포지션을 test_dir으로 함
    QDir::setCurrent("t_unit_dir");
}

Cuma_File_test::~Cuma_File_test ()
{

}

void Cuma_File_test::t_set_File_Frag_Count ()
{
    this->set_File_Frag_Count (10);
}

void Cuma_File_test::t_set_File_Name ()
{
    this->set_File_Name ("test");
    QVERIFY (this->get_File_Name () == "test");
}

void Cuma_File_test::t_set_File_info_block ()
{
    struct Cuma_File_Info_Block i_block;
    i_block.f_Name = "test";
    i_block.f_Binary_Byte = 2000;
    i_block.f_Frag_byte = 30;
    i_block.f_Frag_count = 10;
    
    this->set_File_info_block (i_block);
    struct Cuma_File_Info_Block res_block = this->get_File_info_block ();

    QVERIFY (res_block.f_Name == i_block.f_Name);
    QVERIFY (res_block.f_Binary_Byte == i_block.f_Binary_Byte);
    QVERIFY (res_block.f_Frag_byte == i_block.f_Frag_byte);
    QVERIFY (res_block.f_Frag_count == i_block.f_Frag_count);

}

void Cuma_File_test::t_mf_Read_File_Frag ()
{
    QVERIFY (env_set_env () == 0);

    //파일을 세팅하고 읽음
    set_File_Name ("test.txt");

    //파일을 읽음
    QVERIFY (mf_Read_File () == Cuma_File::C_F_no_err);

    //읽은 바이너리들을 flush함
    clear_binary ();

    //테스트 환경을 flush함
    env_clear_test_env ();
}

void Cuma_File_test::t_mf_Make_Frag ()
{
    QVERIFY (env_set_env () == 0);

    set_File_Name ("test.txt");

    //frag를 나눌 count를 세팅
    set_File_Frag_Count (10);

    //파일 읽기
    QVERIFY (read_file () == Cuma_File::C_F_no_err);

    //파일의 frag를 실행
    QVERIFY (mf_Make_Frag () == 0);

    QVERIFY (get_File_Frag ().count () == 10);

    //파일의 binary를 모두 flush
    clear_binary ();

    //테스트 환경을 flush함
    env_clear_test_env ();
}

void Cuma_File_test::t_mf_Save_by_Frag ()
{
    QVERIFY (env_set_env () == 0);

    set_File_Name ("test.txt");

    //여러개의 파일로 나눔
    set_File_Frag_Count (10);

    read_file ();

    //바이너리 파일을 분활함
    mf_Make_Frag ();

    //분활한 바이너리를 저장
    QVERIFY (mf_Save_by_Frag () == Cuma_File_Status::C_F_no_err);

    //바이너리가 저장되었는지 확인
    QVector<QByteArray> frag_bin = get_File_Frag ();

    QVERIFY (frag_bin.count () == 10);

    QVERIFY (frag_bin[1].size () > 0);

    //바이너리 clear
    clear_binary ();

    //테스트 환경을 flush함
    env_clear_test_env ();
}

void Cuma_File_test::t_mf_Save_by_Frag_QVector ()
{
    // QVector로 파일을 저장하기

    QVERIFY (env_set_env () == 0);

    QFile f_env;
    f_env.setFileName ("test.txt");
    f_env.open (QFile::ReadWrite);

    //f_env_bin을 끝까지 읽음
    QByteArray f_env_bin = f_env.readAll ();
    uint frag_count = 10;
    QVector<QByteArray> sav_bin;

    //파일을 닫음
    f_env.close ();

    uint32_t frag_size = (f_env_bin.count ()/ frag_count);

    //f_env_bin을 count만큼 벡터로 나눔
    for (uint i = 0; i < frag_count; i++)
    {
        sav_bin.append (f_env_bin.mid (i, frag_size));
    }

    //QVector를 저장함
    QVERIFY (mf_Save_by_Frag (sav_bin, "test.txt") == Cuma_File_Status::C_F_no_err);

    //QVector가 저장된것을 확인함
    QVERIFY (get_File_Frag () == sav_bin);

    clear_binary ();

    //테스트 환경을 flush함
    env_clear_test_env ();
}

void Cuma_File_test::t_mf_Save_by_File ()
{
    //파일 바이너리 통째로 저장하기

    QVERIFY (env_set_env () == 0);

    QFile f_env;
    f_env.setFileName ("test.txt");
    f_env.open (QFile::ReadWrite);

    //f_env_bin을 끝까지 읽음
    QByteArray f_env_bin = f_env.readAll ();
    uint32_t index = 0;
    save_File_Frag (f_env_bin, "test.txt", index);

    //바이너리를 저장
    QVERIFY (mf_Save_by_File ("test.txt") == Cuma_File_Status::C_F_no_err);

    //바이너리 클리어
    clear_binary ();

    //테스트 환경을 flush함
    env_clear_test_env ();

}

void Cuma_File_test::t_Save_frag_File ()
{
    QVERIFY (env_set_env () == 0);

    QFile f_env;
    f_env.setFileName ("test.txt");
    f_env.open (QFile::ReadWrite);

    //f_env_bin을 끝까지 읽음
    QByteArray f_env_bin = f_env.readAll ();

    //파일 바이너리를 test.txt로 저장함
    QVERIFY (Save_frag_File ("test.txt", 0, f_env_bin) != nullptr);

    //테스트 환경을 flush함
    env_clear_test_env ();
}

void Cuma_File_test::t_save_File_Frag_QVector ()
{
    QVERIFY (env_set_env () == 0);
    
    QFile f_env;
    f_env.setFileName ("test.txt");
    f_env.open (QFile::ReadWrite);

    //f_env_bin을 끝까지 읽음
    QByteArray f_env_bin = f_env.readAll ();
    
    //파일을 QVector형식으로 저장함
    QVector<QByteArray> sav_bin;

    //파일을 닫음
    f_env.close ();

    sav_bin = env_get_test_frag_binary ("test.txt", 10);

    //sav_bin을 저장함
    save_File_Frag (sav_bin, "test.txt");

    //테스트 환경을 flush함
    env_clear_test_env ();
}

void Cuma_File_test::t_read_File_Frag ()
{
    //파일 frag를 읽기
    QVERIFY (env_set_env () == 0);

    //테스트 파일 의 binary vector를 비교를 위해 저장함
    QByteArray f_array = env_get_test_binary  ("test.txt");
    QVector<QByteArray> f_frag_arr;
    uint32_t f_count = 10;
    //파일 byte frag를 읽기
    QVector<QByteArray> t_file = env_get_test_frag_binary ("test.txt", f_count);

    //파일 이름을 세팅함
    set_File_Name ("test.txt");

    //파일을 생성함
    mf_Save_by_File ("test.txt");

    //파일의 frag count를 생성함
    set_File_Frag_Count (f_count);

    //파일의 frag를 분활함
    QVERIFY (mf_Make_Frag () == Cuma_File_Status::C_F_no_err);

    //파일의 frag를 분활한것을 저장함
    QVERIFY (mf_Save_by_Frag () == 0);

    //파일의 frag를 읽어옴
    for (int i = 0; i < f_count; i++)
    {
        QVERIFY (read_file_frag ("test.txt", i) == Cuma_File_Status::C_F_no_err);
        f_frag_arr.append (get_File_binary ());
    }

    //읽은 파일 바이너리 리스트가 맞는지 확인함
    QVERIFY (f_frag_arr == t_file);

    clear_binary ();
    env_clear_test_env ();
}

void Cuma_File_test::t_read_File ()
{
    //파일을 읽기
    QVERIFY (env_set_env () == 0);

    //테스트 파일을 저장함
    QFile sav_test_file;
    sav_test_file.setFileName ("test.txt");
    sav_test_file.open (QFile::ReadWrite);

    sav_test_file.write (env_get_test_binary  ("test.txt"));
    sav_test_file.close ();

    //파일의 바이너리를 읽음
    set_File_Name ("test_res.txt");
    QVERIFY (read_file () == Cuma_File_Status::C_F_no_err);

    //바이너리를 가짐
    QByteArray ret_binary = get_File_binary ();

    //리턴된 바이너리를 테스트 파일와 비교함
    QVERIFY (env_get_test_binary  ("test.txt") == ret_binary);

    clear_binary();
    env_clear_test_env();
}

void Cuma_File_test::t_clear_save_frag ()
{
    clear_binary();

}

void Cuma_File_test::t_mf_Read_File ()
{
    //파일 frag를 읽기
    QVERIFY (env_set_env () == 0);

    //test_dir에 있는 파일을 t_unit_dir로 저장함
    QByteArray f_array = env_get_test_binary("test.txt");

    //읽을 파일 이름을 set함
    set_File_Name("test.txt");

    //파일의 바이너리를 읽음
    QVERIFY (mf_Read_File () == Cuma_File_Status::C_F_no_err);

    //읽은 파일 바이너리 가 테스트 바이너리와 같은지 확인

    QByteArray read_binary = get_File_binary();
    Cuma_Debug("test binary size = " + QString::number(f_array.size()), __LINE__);
    Cuma_Debug("read binary size = " + QString::number(read_binary.size()), __LINE__);

    QVERIFY (f_array == get_File_binary());

    clear_binary ();
    env_clear_test_env ();

}
































int Cuma_File_test::env_set_env ()
{
    QDir t_f_dir;
    QFile t_f_env;

    if ( t_f_dir.exists ("test_dir") == false)
    {
        t_f_dir.mkdir ("test_dir");
    }

    //테스트 dir에 cd함
    QDir::setCurrent("../test_dir");

    //테스트 텍스트를 생성함
    t_f_env.setFileName ("test.txt");
    QByteArray test_arr;

    //만약 파일이 존재할경우 continue함
    if ( t_f_env.exists () == true )
    {

    }

    //test파일이 존재하지 않을경우 remove함
    else
    {
        t_f_env.open ( QFile::ReadWrite);
        for (qint64 i = 0; i < 1000; i++)
        {
            test_arr.append (QString::number (INT_MAX));
        }
        if (t_f_env.write (test_arr) < 0)
            return -1;

        t_f_env.close ();
    }

    //원래 유닛테스트 dir로 이동
    QDir::setCurrent("../t_unit_dir");

    return 0;


}

QByteArray Cuma_File_test::env_get_test_binary  (QString f_name)
{
    QDir::setCurrent("test_dir");

    QFile t_f_file;
    QByteArray ret_byte;
    t_f_file.setFileName (f_name);
    t_f_file.open (QFile::ReadWrite);
    ret_byte = t_f_file.readAll ();

    QDir::setCurrent("../t_unit_dir");
    return ret_byte;
}

QVector<QByteArray> Cuma_File_test::env_get_test_frag_binary (QString f_name, uint32_t count)
{
    QByteArray f_binary = env_get_test_binary  (f_name);

    if (f_binary == nullptr)
    {
        //만약 nullptr일 경우
        QVector<QByteArray> empty;

        return empty;
    }

    uint32_t frag_size = (f_binary.count ()/ count);

    QVector<QByteArray> bin_vec;

    //f_env_bin을 count만큼 벡터로 나눔
    for (uint i = 0; i < count; i++)
    {
        bin_vec.append (f_binary.mid (i, frag_size));
    }

    return bin_vec;
}

int Cuma_File_test::env_clear_test_env ()
{
    QDir t_f_dir;
    if (t_f_dir.exists ("test_dir") == false)
    {
        return -1;
    }

    //다른 디렉토리들을 전부 flush함

    QFileInfoList dir_lst = t_f_dir.entryInfoList ();

    for (int i = 0; i < dir_lst.count (); i++)
    {
        t_f_dir.remove ( dir_lst.at(i).fileName());
    }

    return 0;
}

int Cuma_File_test::env_mk_file(QByteArray &arr, QString f_name)
{
    //유닛테스트 디렉토리에서 테스트 파일 의 binary vector를 비교를 위해 저장함
    QByteArray f_array = arr;

    QFile sav_test_file;
    sav_test_file.setFileName (f_name);
    sav_test_file.open (QFile::ReadWrite);

    sav_test_file.write (arr);
    sav_test_file.close ();

    return 0;
}

int Cuma_File_test::env_mk_file(QString f_name)
{
    //유닛테스트 디렉토리에서 테스트 파일 의 binary vector를 비교를 위해 저장함
    QByteArray f_array = env_get_test_binary(f_name);

    QFile sav_test_file;
    sav_test_file.setFileName (f_name);
    sav_test_file.open (QFile::ReadWrite);

    sav_test_file.write (f_array);
    sav_test_file.close ();

    return 0;
}

