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

    QVector<QByteArray> get_all_fragment_file();

    QByteArray get_fragment_slite(uint32_t index);

private:

    QSharedPointer<QFile> _Read_File;
    QVector<QByteArray> _File_Frag_Array;

    QString _F_name;

    uint32_t _frag_count;

    unsigned long _file_frag_size;
};

#endif // CUMA_FILE_H
