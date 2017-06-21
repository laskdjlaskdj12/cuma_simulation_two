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

#endif // CUMA_FILE_H
