#include "cuma_file.h"

/*Cuma_File::Cuma_File(QObject *parent) : QObject(parent),
    F_name(nullptr),
    frag_count(0)
{
    _Read_file = new QFile;
}

Cuma_File::~Cuma_File()
{
    _Read_File.clear();
}

void Cuma_File::set_file_frag_count(uint32_t c)
{

}

void Cuma_File::set_read_file_name(QString f)
{
    F_name = f;
}

QString Cuma_File::get_read_file_name()
{
    return F_name;
}

uint32_t Cuma_File::get_frag_count()
{
    if (_File_Frag_Array.count() == 0){ return -1;}
    else { return _File_Frag_Array.count();}
}

int Cuma_File::read_and_fragment()
{
    /*
     * 1. 쪼갤 파일의 인덱스를 읽음
     * 2. 파일을 읽어서 인덱스당 fragment의 크기를 만듬
     * */

/*try
    {
        //오픈할 파일 이름을 읽음
        _Read_File->setFileName(F_name);

        //파일을 READ_ONLY로 오픈함
        if (_Read_File->open(QIODevice::ReadOnly) == false){ throw _Read_File->error();}

        //파일을 읽음
        QByteArray _File_Binary_ = _Read_File->readAll();

        //파일 바이너리를 원하는 count를기준으로 나눔
        _file_frag_size =  static_cast<unsigned long>(_File_Binary_.size() / _frag_count);

        //만약 나머지가 나오면 +1를 함
        if(_File_Binary_.size() % _frag_count != 0){ _frag_count += 1;}

        //파일 를 바이너리 count로 분해함
        for(uint32_t i = 0; i < _frag_count ; i++)
        {
            //파일 프래그먼트를 읽음
            QByteArray temp_fragment;
            temp_fragment((i * _file_frag_size), ((i+1) * _file_frag_size) - 1);

            //읽은 파일 프래그먼트를 push_back함
            _File_Frag_Array.push_back(temp_fragment);
        }
    }
    catch (QFile::FileError& error)
    {
        qDebug()<<"[Error] : File_read_and frag has been error : "<<_Read_File->errorString() <<" Line : "<<__LINE__;
        return error;
    }
}

QByteArray Cuma_File::get_fragment_slite(uint32_t index)
{

}

QString Cuma_File::mf_get_file_error_string()
{
    return _Read_File->errorString();
}

QSharedPointer<Cuma_File> Cuma_File_ReFectoring::get_Cuma_File_Object()
{

}*/

Cuma_File::Cuma_File(QObject *parent = 0):m_File_frag_index(0),\
    m_File_Frag(nullptr),
    m_File_name(nullptr)
{
    try{

        //만약 Cuma_Frag_dir이 없을경우 디렉토리를 만듬
        if(m_Dir.exists("Cuma_Frag_dir") == false)
        {
            if (m_Dir.mkdir("Cuma_Frag_dir") == false)
            {
                //권한 상승이 필요하다는 알림을 리턴
                throw Cuma_Error("CUma_Frag_dir make fail need elevation autholize");
            }
        }

    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
    }
}

Cuma_File::~Cuma_File(){m_File_Frag.clear();}

void Cuma_File::set_File_Frag_Count(uint32_t c){m_File_frag_index = c;}

void Cuma_File::set_File_Name(QString n)
{
    m_File_name = n;
}

int Cuma_File::save_File_Frag(QVector<QByteArray> frag, QString name)
{
    try{
        //만약 File_Frag_index와 File_name이 설정이 안되어있을경우 exception호출
        if(m_File_Frag_Index != nullptr && m_File_Name != nullptr)
        {
            throw Cuma_Error(m_file.errorString(), __LINE__);
        }

        //아니면 mf_Save_Frag()를 실행함
        else
        {
            if ( mf_Save_Frag(frag, name) < 0)
            {
                throw Cuma_Error(m_file.errorString(), __LINE__);
            }
        }

        return Cuma_File_Status::C_F_no_err;
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return Cuma_File_Status::C_F_error;
    }
}

int Cuma_File::save_File_Frag(QByteArray frag, QString name, uint32_t index)
{
    try{
        //만약 File_Frag_index와 File_name이 설정이 안되어있을경우 exception호출
        if(m_File_Frag_Index != nullptr && m_File_Name != nullptr)
        {
            throw Cuma_Error(m_file.errorString(), __LINE__);
        }

        //아니면 mf_Save_Frag()를 실행함
        else
        {
            QVector<QByteArray> save_frag;
            save_frag.append(frag);
            //mf_Save_Frag를 호출해서 파일 인덱스와 frag를 저장
            if ( mf_Save_Frag(save_frag, name, index) < 0)
            {
                throw Cuma_Error(m_file.errorString(), __LINE__);
            }
        }
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
    }
}

uint32_t Cuma_File::get_File_Index()
{
    return m_File_Frag_Index;
}

QString Cuma_File::get_File_Name()
{
    return m_File_Name;
}

QVector<QByteArray> Cuma_File::get_File_Frag()
{
    return m_File_Frag;
}

QByteArray Cuma_File::get_File_Frag_By_Index(uint32_t index)
{
    return m_File_Frag[index];
}

QSharedPointer<Cuma_File> Cuma_File::get_Cuma_File_Object()
{
    return this;
}

bool Cuma_File::find_file_frag(QString file_name, uint32_t index)
{

}

int Cuma_File::mf_Read_File()
{
    try
    {
        //만약 파일이 열려있다면 continue
        if(m_File.isOpen() && m_File.fileName() == m_File_Name)
        {
            continue;
        }
        //만약 파일이 열려있지 않다면
        else
        {
            m_File.close();
            m_File.setFileName(m_File_Name);
            if(m_File.open(QIODevice::ReadWrite) == false)
            {
                throw Cuma_Error(m_File.errorString(), __LINE__);
            }
        }

        //파일 바이너리를 읽음
        m_File_Binary = m_File.readAll();

        return Cuma_File_Status::C_F_no_err;

    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return Cuma_File_Status::C_F_error;
    }
}

int Cuma_File::mf_Make_Frag()
{
    try
    {
        if( ! m_File.isOpen()){ return Cuma_File_Status::C_F_not_open; }

        //파일 바이너리를 원하는 count를기준으로 나눔
        _file_frag_size =  static_cast<unsigned long>(_File_Binary_.size() / _frag_count);

        //만약 나머지가 나오면 +1를 함
        if(_File_Binary_.size() % _frag_count != 0){ _frag_count += 1;}

        //파일 를 바이너리 count로 분해함
        for(uint32_t i = 0; i < _frag_count ; i++)
        {
            //파일 프래그먼트를 읽음
            QByteArray temp_fragment;
            temp_fragment((i * _file_frag_size), ((i+1) * _file_frag_size) - 1);

            //읽은 파일 프래그먼트를 push_back함
            m_File_Frag.push_back(temp_fragment);
        }
    }
    catch (QFile::FileError& error)
    {
        qDebug()<<"[Error] : File_read_and frag has been error : "<<m_File->errorString() <<" Line : "<<__LINE__;
        return Cuma_File_Status::C_F_error;
    }
}

int Cuma_File::mf_Save_by_Frag()
{
    try
    {
        //해당 프레그단위로 여러개의 파일을 만들어서 저장함
        //파일 형식은 JSON형식으로 저장함

        QDir frag_dir = m_Dir;

        if(frag_dir.cd("Cuma_Frag_dir") == false)
        {
            throw Cuma_Error("Can't open Cuma_Frag_Dir", __LINE__);
        }

        for(int i = 0; i < m_File_Frag.count(); i++)
        {
            QFile Frag_File(frag_dir.filePath(m_File_Name + QString::number(i)));

            if (Frag_File.open(QFile::ReadWrite) == false)
            {
                throw Cuma_Error("Frag_File open error", __LINE__);
            }

            Frag_File.write(Save_frag_File(m_File_Name, i, m_File_Frag[i]));
            Frag_File.close();
        }
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();

        if(m_File.error() == QFileDevice::OpenError)
        {
            return Cuma_File_Status::C_F_Exsist;
        }

        return Cuma_File_Status::C_F_error;
    }
}

int Cuma_File::mf_Save_by_Frag(QVector<QByteArray> f_frag, QString f_name, uint32_t f_index)
{

    try
    {
        //해당 프레그단위로 여러개의 파일을 만들어서 저장함
        //파일 형식은 JSON형식으로 저장함

        QDir frag_dir = m_Dir;

        if(frag_dir.cd("Cuma_Frag_dir") == false)
        {
            throw Cuma_Error("Can't open Cuma_Frag_Dir", __LINE__);
        }

        for(int i = 0; i < f_frag.count(); i++)
        {
            QFile Frag_File(frag_dir.filePath(f_name + QString::number(i)));

            if (Frag_File.open(QFile::ReadWrite) == false)
            {
                //파일을 닫음
                Frag_File.close();

                //예외처리 던짐
                throw Cuma_Error("Frag_File open error", __LINE__);
            }

            //파일 내부에 있는 모든 바이너리를 지움
            Frag_File.resize(0);

            //파일의 버퍼를 씀
            Frag_File.write(Save_frag_File(f_name, i, f_frag));

            //파일을 close함
            Frag_File.close();
        }
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();

        if(m_File.error() == QFileDevice::OpenError)
        {
            return Cuma_File_Status::C_F_Exsist;
        }

        return Cuma_File_Status::C_F_error;
    }
}

int Cuma_File::mf_Save_by_File(QString file_name)
{
    //한개의 파일 형식으로 저장함
    //파일은 json형식으로 저장함

    try
    {
        //현재 디렉토리를 멤버 변수에서 가지고옴
        QDir frag_dir = m_Dir;

        //Cuma_Frag_dir로 디렉토리를 변경
        if(frag_dir.cd("Cuma_Frag_dir") == false)
        {
            //만약 없으면 Exception을 리턴
            throw Cuma_Error("Can't open Cuma_Frag_Dir", __LINE__);
        }

        //Cuma_Frag_dir의 디렉토리에 파일이름을 생성 f_name + 인덱스 이터레이션
        QFile Frag_File(frag_dir.filePath(f_name + QString::number(i)));

        //파일을 오픈했는데 만약 파일이 오픈되지 않을 경우 예외 던짐
        if (Frag_File.open(QFile::ReadWrite) == false)
        {
            throw Cuma_Error("Frag_File open error", __LINE__);
        }

        //Save_frag_File로 Json형태의 QBytearray를 쓰기
        Frag_File.write(Save_frag_File(f_name, i, f_frag));

        //파일 닫기
        Frag_File.close();
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();

        if(m_File.error() == QFileDevice::OpenError)
        {
            return Cuma_File_Status::C_F_Exsist;
        }

        return Cuma_File_Status::C_F_error;
    }
}
