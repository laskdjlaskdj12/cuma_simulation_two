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
        _file_frag_size =  static_cast<uint32_t>(_File_Binary_.size() / _frag_count);

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

Cuma_File::Cuma_File(QObject *parent):QObject(parent)
{
    try{
        m_File_Frag_Index = NULL;
        m_File_Name = nullptr;
        m_File_Binary = nullptr;

        //만약 Cuma_Frag_dir이 없을경우 디렉토리를 만듬
        if(m_Dir.exists("Cuma_Frag_dir") == false)
        {
            if (m_Dir.mkdir("Cuma_Frag_dir") == false)
            {
                //권한 상승이 필요하다는 알림을 리턴
                throw Cuma_Error("Cuma_Frag_dir make fail need elevation previleage", __LINE__);
            }
        }

        //dir의 현재 위치를 넣음
        u_root_path = m_Dir.absolutePath();
        u_c_f_path = u_root_path + "Cuma_Frag_dir";
    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
    }
}

Cuma_File::~Cuma_File(){
    m_File_Frag.clear();
    m_File.close();
}

void Cuma_File::set_File_Frag_Count(uint32_t c){m_File_Frag_Index = c;}

void Cuma_File::set_File_Name(QString n)
{
    m_File_Name = n;
}

void Cuma_File::set_File_info_block(struct Cuma_File_Info_Block& block)
{
    m_File_info_Block = block;
}

int Cuma_File::save_File_Frag(QVector<QByteArray> frag, QString name)
{
    try{

        //만약 File_Frag_index와 File_name이 설정이 안되어있을경우 exception호출
        if(m_File_Frag_Index != 0 && m_File_Name != NULL)
        {
            throw Cuma_Error("File Frag_cache is emtpy", __LINE__);
        }

        //아니면 mf_Save_by_Frag()를 실행함
        else
        {
            if ( mf_Save_by_Frag(frag, name) < 0)
            {
                throw Cuma_Error(m_File.errorString(), __LINE__);
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
        if(m_File_Frag_Index != 0 && m_File_Name != nullptr)
        {
            throw Cuma_Error(m_File.errorString(), __LINE__);
        }

        //아니면 mf_Save_by_Frag()를 실행함
        else
        {
            QVector<QByteArray> save_frag;
            save_frag.append(frag);
            //mf_Save_by_Frag를 호출해서 파일 인덱스와 frag를 저장
            if ( mf_Save_by_Frag(save_frag, name, index) < 0)
            {
                throw Cuma_Error(m_File.errorString(), __LINE__);
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

QByteArray Cuma_File::get_File_Frag_By_Index(uint32_t& index)
{
    return m_File_Frag[index];
}

QSharedPointer<Cuma_File> Cuma_File::get_Cuma_File()
{
    QSharedPointer<Cuma_File> ret_obj = QSharedPointer<Cuma_File>(this);
    return ret_obj;
}

QByteArray Cuma_File::get_File_binary()
{
    return m_File_Binary;
}

Cuma_File_Info_Block Cuma_File::get_File_info_block()
{
    return m_File_info_Block;
}

int Cuma_File::read_file_frag (QString file_name, uint32_t index)
{
    m_File_Name = file_name;
    m_File_Frag_Index = index;

    return mf_Read_File_Frag();
}

int Cuma_File::read_file()
{
    if(m_File_Name != nullptr || m_File_Frag_Index )
        return mf_Read_File();
}

void Cuma_File::clear_binary()
{
    m_File.close();
    m_File.flush();
    m_File_Name.clear();
    m_File_Frag_Index = NULL;
    m_File_Frag.clear();
    m_File_Binary.clear();
    m_File_info_Block.Block_size = 0;
    m_File_info_Block.f_Binary_Byte = 0;
    m_File_info_Block.f_Frag_count = 0;
    m_File_info_Block.f_Name = "NULL";
}

int Cuma_File::mf_Read_File()
{
    try
    {
        Cuma_Debug("Start mf_Read_File", __LINE__);

        if(m_File.isOpen() == false || m_File.fileName() != m_File_Name)
        {
            Cuma_Debug("File is not open reopen", __LINE__);

            m_File.close();
            m_File.setFileName(m_File_Name);
            if(m_File.open(QIODevice::ReadOnly) == false)
            {
                throw Cuma_Error(m_File.errorString(), __LINE__);
            }
        }

        Cuma_Debug("start read File_binary", __LINE__);
        //파일 바이너리를 읽음
        m_File_Binary = m_File.readAll();

        Cuma_Debug("read binary size :"+ QString::number(m_File_Binary.size()), __LINE__);


        Cuma_Debug("check byte is maximum", __LINE__);
        //만약 바이너리가 UINT_MAX일 경우 C_F_READ_FILE_SIZE_MAXIMUM 경고를 리턴함
        if(m_File_Binary.count() > UINT_MAX)
        {
            return Cuma_File_Status::C_F_READ_FILE_SIZE_MAXIMUM;
        }

        Cuma_Debug("add file binary info to m_Fileinfo_block", __LINE__);
        //파일 바이너리정보를 m_File_info_Block에 넣음
        m_File_info_Block.f_Binary_Byte = m_File_Binary.size();
        m_File_info_Block.f_Name = m_File_Name;

        return Cuma_File_Status::C_F_no_err;

    }
    catch(Cuma_Error& e)
    {
        e.show_error_string();
        return Cuma_File_Status::C_F_error;
    }
}

int Cuma_File::mf_Read_File_Frag()
{
    try{

        uint32_t frag_count = 0;

        frag_count = m_File_Frag_Index;

        m_File.setFileName(u_c_f_path + m_File_Name + QString::number(m_File_Frag_Index));

        //만약 파일이 없을 경우 throw 함
        if(QFile::exists(u_c_f_path + m_File_Name + QString::number(m_File_Frag_Index)) == false)
        {
            throw Cuma_File::C_F_NOT_EXSIST;
        }

        //만약 파일이 오픈이 안되어있을시 open을 함
        if(m_File.isOpen() == false)
        {
            if(m_File.open(QFile::ReadOnly) == false)
            {
                throw m_File.error();
            }
        }

        m_File_Binary =  m_File.readAll();

        if(m_File_Binary.isEmpty())
        {
            throw m_File.error();
        }

        return Cuma_File_Status::C_F_no_err;
    }
    catch(QString& e)
    {
        qDebug()<<"[Error] : "<< e;

        if(e == QFileDevice::FileError::OpenError){ return Cuma_File_Status::C_F_not_open;}
        else { return -1;}
    }
    catch(Cuma_File::Cuma_File_Status& e)
    {
        Cuma_Debug(m_File_Name + QString::number(m_File_Frag_Index) + "is not exsist");
        return e;
    }
}

int Cuma_File::mf_Make_Frag()
{
    try
    {
        uint32_t _file_frag_size = 0;
        uint32_t _frag_count = 0 ;

        Cuma_Debug("check m_File is open", __LINE__);
        if( ! m_File.isOpen()){ return Cuma_File_Status::C_F_not_open; }

        Cuma_Debug("frag file bianry", __LINE__);

        _frag_count = m_File_Frag_Index;
        //파일 바이너리를 원하는 count를기준으로 나눔
        _file_frag_size =  static_cast<uint32_t>(m_File_Binary.size() / _frag_count);
        Cuma_Debug("file_frag_size = " + QString::number(_file_frag_size), __LINE__);

        //만약 나머지가 나오면 +1를 함
        if(m_File_Binary.size() % _frag_count != 0){ _frag_count += 1;}
        Cuma_Debug("file_frag_size = " + QString::number(_file_frag_size), __LINE__);

        Cuma_Debug("starting frag file", __LINE__);
        //파일 를 바이너리 count로 분해함
        for(uint32_t i = 0; i < _frag_count ; i++)
        {
            //파일 프래그먼트를 읽음
            Cuma_Debug("frag file index = " + QString::number(i), __LINE__);
            QByteArray temp_fragment;

            temp_fragment.append(m_File_Binary.mid(i, _file_frag_size));

            Cuma_Debug("frag_size : " + QString::number(temp_fragment.size ()), __LINE__);
            //읽은 파일 프래그먼트를 push_back함
            m_File_Frag.push_back(temp_fragment);
        }

        Cuma_Debug("end of frag file ", __LINE__);
        return Cuma_File_Status::C_F_no_err;
    }

    catch (QFile::FileError& error)
    {
        qDebug()<<"[Error] : File_read_and frag has been error : "<<m_File.errorString() <<" Line : "<<__LINE__;
        return Cuma_File_Status::C_F_error;
    }
    catch (QException& e)
    {
        qDebug()<<"[Error] : " << e.what();
        return Cuma_File_Status::C_F_error;
    }
}

int Cuma_File::mf_Save_by_Frag()
{
    try
    {
        //해당 프레그단위로 여러개의 파일을 만들어서 저장함
        //파일 형식은 JSON형식으로 저장함

        Cuma_Debug("change directory to Cuma_Frag_dir", __LINE__);

        Cuma_Debug("current path : " + u_c_f_path);

        Cuma_Debug("write frag file ", __LINE__);
        for(int i = 0; i < m_File_Frag.count(); i++)
        {
            QFile Frag_File;
            Frag_File.setFileName(u_c_f_path + m_File_Name + QString::number(i));
            Cuma_Debug("file path = "+ u_c_f_path + "\\"+ m_File_Name + QString::number(i), __LINE__);

            //파일이 안열려있으면 파일을 오픈함
            if(Frag_File.isOpen() == false)
            {
                Cuma_Debug("file is not open", __LINE__);
                Cuma_Debug("change directory", __LINE__);
                if (Frag_File.open(QFile::WriteOnly) == false)
                {
                    throw Cuma_Error("Frag_File open error", __LINE__);
                }
            }

            //만약 쓰기가 에러가 났을 경우 Cuma_error를 emit함
            if (Frag_File.write(Save_frag_File(m_File_Name, i, m_File_Frag[i])) < 0)
            {
                throw Cuma_Error("Write Error : "+ Frag_File.errorString(), __LINE__);
            }
            Frag_File.close();
        }
        return Cuma_File_Status::C_F_no_err;

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

        Cuma_Debug("set Frag save location to Cuma_Frag_dir is clear", __LINE__);

        //파일 frag 카운트 대로 저장함
        for(int i = 0; i < f_frag.count(); i++)
        {
            //해당 파일 이름을 (frag 이름 + frag 인덱스)으로 생성함
            QFile Frag_File(u_c_f_path + (f_name + QString::number(i)));
            Cuma_Debug("generate file to Cuma_Frag_dir absolutePath is clear", __LINE__);

            //파일을 ReadWrtie로 오픈함
            Cuma_Debug("open Frag_File", __LINE__);
            if (Frag_File.open(QFile::ReadWrite) == false)
            {
                Cuma_Debug("Error of open File.", __LINE__);
                //파일을 닫음
                Frag_File.close();

                //예외처리 던짐
                throw Cuma_Error("Frag_File open error", __LINE__);
            }
            Cuma_Debug("open Frag_File is clear", __LINE__);

            //파일의 버퍼를 씀
            Frag_File.write(Save_frag_File(f_name, i, f_frag[i]));
            Cuma_Debug("write file buffer is clear", __LINE__);

            //파일을 close함
            Frag_File.close();
            Cuma_Debug("close Frag_File is clear", __LINE__);
        }

        return Cuma_File_Status::C_F_no_err;
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
    //현재 남아있는 모든 파일 frag들을 통해 file_name으로 만들기
    try
    {
        //만약 파일이 오픈이 되어있다면 close함
        if(m_File.isOpen())
        {
            m_File.close();
        }
        Cuma_Debug("init mFile", __LINE__);
        m_File.setFileName(file_name);

        Cuma_Debug("open m_File", __LINE__);
        //만약 파일오픈이 안될경우 throw함
        if (m_File.open(QFile::ReadWrite) == false)
        {
            throw m_File.error();
        }
        Cuma_Debug("open m_File is clear", __LINE__);

        //캐시된 파일 frag의 모든 바이트를 읽을 카운트
        uint32_t All_File_Frag_count;
        QByteArray Save_File_Binary;

        Cuma_Debug("read file frag count", __LINE__);
        foreach(const QByteArray& arr, m_File_Frag)
        {
            All_File_Frag_count += arr.size();
        }

        Cuma_Debug("compare all fragment and make one file binary", __LINE__);
        //QByteArray에 모든 바이트들을 취합함
        for(int i = 0; i < m_File_Frag.count(); i++)
        {
            Save_File_Binary += m_File_Frag[i];
        }

        Cuma_Debug("make file ", __LINE__);
        //파일을 만듬
        m_File.write(Save_File_Binary);

        Cuma_Debug("close m_File", __LINE__);
        //파일을 close함
        m_File.close();

        return Cuma_File_Status::C_F_no_err;
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

QByteArray Cuma_File::Save_frag_File(QString f_name, uint32_t f_index, QByteArray f_binary)
{
    QJsonObject file_binary;
    file_binary["f_name"] = f_name;
    file_binary["f_index"] = static_cast<int>(f_index);
    file_binary["f_binary"] = QString(f_binary);

    QJsonDocument doc(file_binary);

    return doc.toJson();
}

QSharedPointer<QFile> Cuma_File::mf_Open_Frag_File(QString file_name)
{
    QSharedPointer<QFile> file = QSharedPointer<QFile>(new QFile);

    file->setFileName(u_c_f_path + file_name);
    if(file->open(QFile::ReadWrite) == false)
    {
        Cuma_Debug("File open Error: file name = " + u_c_f_path + file_name, __LINE__);
    }

    return file;
}
