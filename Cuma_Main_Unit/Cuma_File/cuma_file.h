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
    explicit Cuma_File(QObject* parent = 0);
    ~Cuma_File();

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
    virtual QSharedPointer<Cuma_File> get_Cuma_File_Object();

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
