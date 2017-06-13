#include "cuma_file.h"

Cuma_File::Cuma_File(QObject *parent) : QObject(parent),
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

    try
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

QVector<QByteArray> Cuma_File::get_all_fragment_file()
{

}

QByteArray Cuma_File::get_fragment_slite(uint32_t index)
{

}
