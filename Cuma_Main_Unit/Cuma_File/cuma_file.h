#ifndef CUMA_FILE_H
#define CUMA_FILE_H

#include <QObject>
#include <QSharedPointer>
#include <QFile>
#include <QVector>
#include <QByteArray>

class Cuma_File : public QObject
{
    Q_OBJECT
public:
    explicit Cuma_File(QObject *parent = nullptr);

    ~Cuma_File();

    void set_file_frag_count(uint32_t c);

    void set_read_file_name(QString f);

    QString get_read_file_name();

    uint32_t get_frag_count();

    unsigned long each_fragment_size;

    int read_and_fragment();

    void read_file_frag(QByteArray array, uint32_t index);

    QVector<QByteArray> get_all_fragment_file();

    QByteArray get_fragment_slite(uint32_t index);

    QString mf_get_file_error_string();

protected:

    //파일을 읽어오기
    virtual int read_file(QString f_name);

    //파일을 쓰기
    virtual int write_file(QByteArray arr);

    //파일 파편화
    virtual int frag_file();

    //파일 파편화 저장
    virtual int save_file_frag();

    //파일 파편화 읽어오기
    virtual QByteArray read_file_frag(uint32_t index);

private:

    QSharedPointer<QFile> _Read_File;

    QVector<QByteArray> _File_Frag_Array;

    QString _F_name;

    QString _F_Path;

    uint32_t _frag_count;

    unsigned long _file_frag_size;
};

class Cuma_File_ReFectoring: public QObject
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

protected:
    virtual int read_file();
    virtual int make_frag();
    virtual int save_Frag(QVector<QByteArray> frag, QString name, uint32_t index = NULL);

private:
    QString File_name;
    uint32_t File_frag;
    QVector<QByteArray> File_Frag;
};

#endif // CUMA_FILE_H
