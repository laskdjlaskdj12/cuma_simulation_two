#ifndef CUMA_FILE_TEST_ENV_H
#define CUMA_FILE_TEST_ENV_H

#include <QObject>
#include "../Cuma_Main_Unit/Cuma_File/cuma_file.h"
#include "../Cuma_Main_Unit/Cuma_File/cuma_file_info_struct.h"

class t_Cuma_File : public Cuma_File
{
    t_Cuma_File();
    ~t_Cuma_File();

    virtual QSharedPointer<Cuma_File> get_Cuma_File();

    void set_name(QString name);
    void set_frag_index(uint32_t idx);
    void set_f_info_block(struct Cuma_File_Info_Block& block);
    void read_frag(uint32_t idx, QString name);
    void save_frag(QByteArray& b, QString name, uint32_t idx);

};

#endif // CUMA_FILE_TEST_ENV_H
