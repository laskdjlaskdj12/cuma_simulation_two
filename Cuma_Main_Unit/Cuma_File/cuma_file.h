#ifndef CUMA_FILE_H
#define CUMA_FILE_H

#include <QObject>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QSharedPointer>
#include <QMutex>
#include "../../Cuma_Debug/cuma_debug.h"
#include "cuma_file_info_struct.h"
#include <QException>

#define Cuma_Debug_str(X) Cuma_Debug(X, __LINE__)

class Cuma_File : public QObject
{
    Q_OBJECT
public:
    enum Cuma_File_Status : int
    {
        C_F_error = -1,
        C_F_not_open = -2,
        C_F_Exsist = -3,
        C_F_Dir_Not_Open = -4,
        C_F_READ_FILE_SIZE_MAXIMUM = -5,
        C_F_NOT_EXSIST = -6,
        C_F_no_err = 0
    };

public:
    Cuma_File(QObject* parent = 0);
    Cuma_File(Cuma_File& c);
    ~Cuma_File();

    virtual void set_File_Frag_Count(uint32_t c);
    virtual void set_File_Name(QString n);
    virtual void set_File_info_block(struct Cuma_File_Info_Block& block);

    virtual uint32_t                get_File_Index();
    virtual QString                 get_File_Name();
    virtual QVector<QByteArray>     get_File_Frag();
    virtual QByteArray              get_File_Frag_By_Index(uint32_t& index);
    virtual QSharedPointer<Cuma_File> get_Cuma_File();
    virtual QByteArray              get_File_binary();
    virtual struct Cuma_File_Info_Block get_File_info_block();

    virtual int read_file_frag(QString file_name, uint32_t index);
    virtual int read_file();

    virtual int save_File_Frag(QVector<QByteArray> frag, QString name);
    virtual int save_File_Frag(QByteArray frag, QString name, uint32_t index);

    virtual void clear_binary();


    /*
     * ========== useage ==========
     * 1. 해당 class는 파일 바이너리를 클래스 멤버 변수로 두고있어 캐시가 잔류해 있음
     * 2. Cuma_File_Info_Block은 파일을 읽을때 부터 생성함 여기에 파일 인덱스 정보들이 들어가있음
     * 3. Cuma_File_Info_Block에는 파일 frag의 info와 파일 info 가 들어가 있음
     *
     *
     * ========= useage =========
     * 1. frag를 저장할때
     * 1-1. save_File_Frag()으로 저장
     *
     * 2. 파일을 읽어올때
     * 2-1. set_File_Name();
     * 2-2. read_file();
     *
     * 3. 파일 frag를 읽어올대
     * 3-1. get_File_Frag_By_Index 으로 인덱스를 설정
     * 3-2. get_File_Frag_By_Index()으로 리턴됨 바이너리 값을 받음
     *
     *
     * */
protected:
    virtual int mf_Read_File();
    virtual int mf_Read_File_Frag();
    virtual int mf_Make_Frag();
    virtual int mf_Save_by_Frag();
    virtual int mf_Save_by_Frag(QVector<QByteArray> frag, QString name, uint32_t index = NULL);
    virtual int mf_Save_by_File(QString file_name);

protected:
    static QByteArray Save_frag_File(QString f_name, uint32_t f_index, QByteArray f_binary);

protected:
    QSharedPointer<QFile> mf_Open_Frag_File(QString file_name);

private:
    QDir m_Dir;
    QFile m_File;
    QString m_File_Name;
    uint32_t m_File_Frag_Index;
    QVector<QByteArray> m_File_Frag;
    QByteArray m_File_Binary;

    struct Cuma_File_Info_Block m_File_info_Block;

private:
    static QMutex m_Mutex;

private:
    QString u_root_path;
    QString u_c_f_path;



};


#endif // CUMA_FILE_H
