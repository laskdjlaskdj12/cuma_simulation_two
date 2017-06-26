#ifndef CUMA_FILE_H
#define CUMA_FILE_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QSharedPointer>
#include <QMutex>
#include "../../Cuma_Debug/cuma_debug.h"

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
        C_F_no_err = 0
    };

public:
    explicit Cuma_File(QObject* parent = 0);
    ~Cuma_File();

    virtual void set_File_Frag_Count(uint32_t c);
    virtual void set_File_Name(QString n);
    virtual int save_File_Frag(QVector<QByteArray> frag, QString name);
    virtual int save_File_Frag(QByteArray frag, QString name, uint32_t index);

    virtual uint32_t get_File_Index();
    virtual QString get_File_Name();
    virtual QVector<QByteArray> get_File_Frag();
    virtual QByteArray get_File_Frag_By_Index(uint32_t index);
    virtual QSharedPointer<Cuma_File> get_Cuma_File_Object();
    virtual QByteArray get_File_binary();

    virtual int read_file_frag(QString file_name, uint32_t index);

protected:
    virtual int mf_Read_File();
    virtual int mf_Read_File_Frag();
    virtual int mf_Make_Frag();
    virtual int mf_Save_by_Frag();
    virtual int mf_Save_by_Frag(QVector<QByteArray> frag, QString name, uint32_t index = NULL);
    virtual int mf_Save_by_File(QString file_name);

protected:
    static QByteArray Save_frag_File(QString f_name, uint32_t f_index, QByteArray f_binary);

private:
    QString m_File_Name;
    uint32_t m_File_Frag_Index;
    QVector<QByteArray> m_File_Frag;
    QByteArray m_File_Binary;
    QFile m_File;
    QDir m_Dir;

private:
    static QMutex m_Mutex;


};

/*class Cuma_File_ReFectoring: public QObject
{
    Q_OBJECT
public:
    explicit Cuma_File_ReFectoring(QObject* parent = 0);
    ~Cuma_File_ReFectoring();

    virtual void set_File_Frag_Count(uint32_t c);
    virtual void set_File_Name(QString n);
    virtual void save_File_Frag(QVector<QByteArray> frag, QString name);
    virtual void save_File_Frag(QVector<QByteArray> frag, QString name, uint32_t index);

    virtual uint32_t get_File_Index();
    virtual QString get_File_Name();
    virtual QVector<QByteArray> get_File_Frag();
    virtual QByteArray get_File_Frag_By_Index(uint32_t index);
    virtual QSharedPointer<Cuma_File> get_Cuma_File_Object();

protected:
    virtual int read_file();
    virtual int make_frag();
    virtual int save_Frag(QVector<QByteArray> frag, QString name, uint32_t index = NULL);

private:
    QString File_name;
    uint32_t File_frag;
    QVector<QByteArray> File_Frag;
};*/

/*class Cuma_File_Format
{
public:
    static QByteArray Save_frag_File(QString f_name, uint32_t f_index, QByteArray f_binary);
};*/

#endif // CUMA_FILE_H
