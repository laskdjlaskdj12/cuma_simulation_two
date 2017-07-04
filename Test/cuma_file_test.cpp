#include "cuma_file_test.h"

Cuma_File_test::Cuma_File_test (QObject *parent)
{
    QDir dir;

    Cuma_Debug::show_debug(true);

    root_path = QDir::currentPath();

    //test할 디렉토리를 삭제하고 새로 생성함

    QStringList entry_lst = dir.entryList();

    for(QString& s : entry_lst)
    {
        if(s == "t_unit_dir")
        {
            dir.remove(s);
        }
        else if( s == "Cuma_Frag_dir")
        {
            dir.remove(s);
        }
    }

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


void Cuma_File_test::t_mf_Read_File ()
{
    //테스트 환경 정리
    env_clear_test_env ();

    //파일 frag를 읽기
    QVERIFY (env_set_env () == 0);

    //현재 위치를 테스트 위치로 변경함
    QDir::setCurrent(root_path + "/t_unit_dir");

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
    QDir::setCurrent(root_path);
}



void Cuma_File_test::t_mf_Read_File_Frag ()
{
    //테스트 환경 정리
    env_clear_test_env ();

    QVERIFY (env_set_env () == 0);

    //현재 위치를 테스트 위치로 변경함
    QDir::setCurrent(root_path + "/t_unit_dir");

    //현재 위치에서 파일을 만듬
    env_mk_file("test.txt");

    //파일들을 frag형태로 나눠서 파일명 + 번호 형식으로 나눔
    QVector<QByteArray> arr = env_get_test_frag_binary("test.txt", 10);

    Cuma_Debug("arr.count == "+ QString::number(arr.count()), __LINE__);

    Cuma_Debug("test env is clear", __LINE__);

    Cuma_Debug("generate make file is clear", __LINE__);
    //파일을 생성함
    for(int i = 0; i < arr.count() ; i++)
    {
        QVERIFY (env_mk_file(arr[i], "test.txt" + QString::number(i)) == 0);
    }
    Cuma_Debug("generate make file is clear", __LINE__);

    for(int i = 0; i < 10 ; i++)
    {
        Cuma_Debug("set file name", __LINE__);
        //파일을 세팅하고 읽음
        set_File_Name ("test.txt");

        Cuma_Debug("set frag count = " + QString::number(i), __LINE__);
        set_File_Frag_Count(i);

        Cuma_Debug("read file frag" + QString::number(i), __LINE__);
        //파일을 읽음
        QVERIFY (mf_Read_File_Frag () == Cuma_File::C_F_no_err);

        Cuma_Debug("check file is same as compare test file " + QString::number(i), __LINE__);
        //해당 파일이 arr와 같은지 확인함
        QVERIFY(arr[i] == get_File_binary());

        Cuma_Debug("clear binary", __LINE__);

        //테스트 환경 정리
        clear_binary ();
    }

    env_clear_test_env ();
    QDir::setCurrent(root_path);
}

void Cuma_File_test::t_mf_Make_Frag ()
{
    //테스트 환경 정리
    env_clear_test_env ();

    QVERIFY (env_set_env () == 0);

    //현재 위치를 테스트 위치로 변경함
    QDir::setCurrent(root_path + "/t_unit_dir");

    //현재 위치에서 파일을 만듬
    env_mk_file("test.txt");

    Cuma_Debug("test env is clear", __LINE__);

    set_File_Name("test.txt");
    Cuma_Debug("set File name is clear", __LINE__);

    set_File_Frag_Count(10);
    Cuma_Debug("set File count is clear", __LINE__);

    //파일을 저장함
    QVERIFY (read_file() == Cuma_File_Status::C_F_no_err);
    Cuma_Debug("read file binary is clear", __LINE__);

    //파일을 frag함
    QVERIFY (mf_Make_Frag() == Cuma_File_Status::C_F_no_err);
    Cuma_Debug("frag fila binary is clear", __LINE__);

    Cuma_Debug("check file frag is 10", __LINE__);
    //frag한 파일 수가 10인지 확인
    QVERIFY (get_File_Frag ().count () == 10);

    Cuma_Debug("flush file binary", __LINE__);
    //파일의 binary를 모두 flush
    clear_binary ();

    Cuma_Debug("flush test env", __LINE__);
    //테스트 환경을 flush함
    env_clear_test_env ();

    QDir::setCurrent(root_path);
}

void Cuma_File_test::t_mf_Save_by_Frag ()
{
    //테스트 환경 정리
    env_clear_test_env ();

    QVERIFY (env_set_env () == 0);

    //현재 위치를 테스트 위치로 변경함
    if (QDir::setCurrent(root_path + "/t_unit_dir") == false)
    {
        Cuma_Debug("set test location dir is fail", __LINE__);
    }

    //현재 위치에서 파일을 만듬
    env_mk_file("test.txt");

    Cuma_Debug("test env is clear", __LINE__);

    //여러개의 파일로 나눔
    set_File_Frag_Count (10);
    Cuma_Debug("set file frag count", __LINE__);

    set_File_Name("test.txt");
    Cuma_Debug("set file name", __LINE__);

    //파일을 읽음
    QVERIFY(read_file () == Cuma_File_Status::C_F_no_err);
    Cuma_Debug("read file binary is clear", __LINE__);

    //frag를 만듬
    QVERIFY (mf_Make_Frag() == Cuma_File_Status::C_F_no_err);
    Cuma_Debug("make frag is clear", __LINE__);

    //파일을 frag단위로 저장함
    QVERIFY(mf_Save_by_Frag() == Cuma_File_Status::C_F_no_err);
    Cuma_Debug("save file by frag", __LINE__);

    Cuma_Debug("print file list", __LINE__);

    //파일 리스트를 출력함
    QDir F_list;
    F_list.setPath(root_path + "/t_unit_dir/Cuma_Frag_dir/");

    QStringList Entry_list = F_list.entryList();

    for(int i = 0; i < Entry_list.count(); i++)
    {
        Cuma_Debug("File_List :" + Entry_list[i]);
    }

    Cuma_Debug("clear binary env", __LINE__);
    //바이너리 clear
    clear_binary ();

    //테스트 환경을 flush함
    env_clear_test_env ();

    QDir::setCurrent(root_path);

}

void Cuma_File_test::t_mf_Save_by_Frag_QVector ()
{
    //테스트 환경 정리
    env_clear_test_env ();

    // QVector로 파일을 저장하기
    QVERIFY (env_set_env () == 0);

    //현재 위치를 테스트 위치로 변경함
    if (QDir::setCurrent(root_path + "/t_unit_dir") == false)
    {
        Cuma_Debug("set test location dir is fail", __LINE__);
    }

    //현재 위치에서 파일을 만듬
    env_mk_file("test.txt");

    Cuma_Debug("test env is clear", __LINE__);


    uint frag_count = 10;
    QVector<QByteArray> sav_bin = env_get_test_frag_binary("test.txt", frag_count);
    Cuma_Debug("Generate env get_test_frag_binary", __LINE__);

    //QVector를 저장함
    QVERIFY (mf_Save_by_Frag (sav_bin, "test.txt") == Cuma_File_Status::C_F_no_err);
    Cuma_Debug("mf_Save_by_Frag is clear", __LINE__);

    //QVector가 저장된것을 확인함
    QVERIFY (get_File_Frag () == sav_bin);
    Cuma_Debug("compare get_File_Frag is same as sav_bin", __LINE__);

    clear_binary ();

    //테스트 환경을 flush함
    env_clear_test_env ();
    QDir::setCurrent(root_path);
}


// ============= 클래스 메소드를 위주로 테스트를 진행함 중복으로 테스트가 실행된 메소드가 없도록 함 //

void Cuma_File_test::t_mf_Save_by_File ()
{
    //테스트 환경 정리
    env_clear_test_env ();

    //파일 바이너리 통째로 저장하기
    //현재 위치를 테스트 위치로 변경함
    if (QDir::setCurrent(root_path + "/t_unit_dir") == false)
    {
        Cuma_Debug("set test location dir is fail", __LINE__);
    }

    QVERIFY (env_set_env () == 0);

    QFile f_env;
    f_env.setFileName ("test.txt");
    f_env.open (QFile::ReadWrite);

    //f_env_bin을 끝까지 읽음
    QByteArray f_env_bin = f_env.readAll ();
    uint32_t index = 0;
    save_File_Frag (f_env_bin, "test.txt", index);

    //바이너리를 저장
    QVERIFY (mf_Save_by_File ("test_res.txt") == Cuma_File_Status::C_F_no_err);

    QFile c_file("test_res.txt");
    QVERIFY (c_file.open(QFile::ReadWrite) == true);

    QByteArray f_bin = c_file.readAll();

    QVERIFY(f_env_bin == f_bin);

    //바이너리 클리어
    clear_binary ();

    //테스트 환경을 flush함
    env_clear_test_env ();
    QDir::setCurrent(root_path);

}

void Cuma_File_test::t_Save_frag_File ()
{
    //테스트 환경 정리
    env_clear_test_env ();

    QVERIFY (env_set_env () == 0);

    if (QDir::setCurrent(root_path + "/t_unit_dir") == false)
    {
        Cuma_Debug("set test location dir is fail", __LINE__);
    }

    QFile f_env;
    f_env.setFileName ("test.txt");
    f_env.open (QFile::ReadWrite);

    //f_env_bin을 끝까지 읽음
    QByteArray f_env_bin = f_env.readAll ();

    //파일 바이너리를 test.txt로 저장함
    QVERIFY (Save_frag_File ("test.txt", 0, f_env_bin) != nullptr);

    //테스트 환경을 flush함
    env_clear_test_env ();
    QDir::setCurrent(root_path);
}

void Cuma_File_test::t_save_File_Frag_QVector ()
{
    //테스트 환경 정리
    env_clear_test_env ();

    QVERIFY (env_set_env () == 0);
    
    if (QDir::setCurrent(root_path + "/t_unit_dir") == false)
    {
        Cuma_Debug("set test location dir is fail", __LINE__);
    }

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
    QDir::setCurrent(root_path);
}

void Cuma_File_test::t_read_File_Frag ()
{
    //테스트 환경 정리
    env_clear_test_env ();

    //파일 frag를 읽기
    QVERIFY (env_set_env () == 0);

    if (QDir::setCurrent(root_path + "/t_unit_dir") == false)
    {
        Cuma_Debug("set test location dir is fail", __LINE__);
    }

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

    //파일을 읽음
    QVERIFY (mf_Read_File() == Cuma_File_Status::C_F_no_err);

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
    QDir::setCurrent(root_path);
}

void Cuma_File_test::t_read_File ()
{
    //테스트 환경 정리
    env_clear_test_env ();

    //파일을 읽기
    QVERIFY (env_set_env () == 0);

    //현재 위치를 테스트 위치로 변경함
    QDir::setCurrent("t_unit_dir");

    //테스트 파일을 저장함
    env_mk_file("test.txt");

    //파일의 바이너리를 읽음
    set_File_Name ("test.txt");
    QVERIFY (read_file () == Cuma_File_Status::C_F_no_err);

    //바이너리를 가짐
    QByteArray ret_binary = get_File_binary ();

    //리턴된 바이너리를 테스트 파일와 비교함
    QVERIFY (env_get_test_binary  ("test.txt") == ret_binary);

    clear_binary();
    env_clear_test_env();
    //현재 위치를 테스트 위치로 변경함
    QDir::setCurrent(root_path);
}

void Cuma_File_test::t_clear_save_frag ()
{
    clear_binary();

}






























int Cuma_File_test::env_set_env ()
{
    QDir t_f_dir;
    QFile t_f_env;

    QDir::setCurrent(root_path);

    if ( t_f_dir.exists ("test_dir") == false)
    {
        t_f_dir.mkdir ("test_dir");
    }

    //테스트 dir에 cd함
    QDir::setCurrent("test_dir");

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
    QDir::setCurrent(root_path);

    return 0;


}

QByteArray Cuma_File_test::env_get_test_binary  (QString f_name)
{
    QFile t_f_file;
    QByteArray ret_byte;
    t_f_file.setFileName (root_path + "/test_dir/" + f_name);
    t_f_file.open (QFile::ReadWrite);
    ret_byte = t_f_file.readAll ();
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

    uint32_t frag_size = (f_binary.size ()/ count);

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
    if(QDir::currentPath() != root_path)
    {
        QDir::setCurrent(root_path);
    }

    //디렉토리들을 전부 제거하고 다시생성함

    QDir dir;
    dir.remove("t_unit_dir");
    dir.mkdir("t_unit_dir");

    QDir::setCurrent("t_unit_dir");

    return 0;
}

int Cuma_File_test::env_mk_file(QByteArray &arr, QString f_name)
{
    //유닛테스트 디렉토리에서 테스트 파일 의 binary vector를 비교를 위해 저장함

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

