#include "cuma_file_test_env.h"

t_Cuma_File::t_Cuma_File()
{

}

t_Cuma_File::~t_Cuma_File()
{

}

QSharedPointer<Cuma_File> t_Cuma_File::get_Cuma_File()
{
   // QSharedPointer<Cuma_File> ret_obj = QSharedPointer<Cuma_File>(this);
   // return ret_obj;

    return nullptr;
}

void t_Cuma_File::set_name(QString name)
{

}

void t_Cuma_File::set_frag_index(uint32_t idx)
{

}

void t_Cuma_File::set_f_info_block(Cuma_File_Info_Block &block)
{

}

void t_Cuma_File::read_frag(uint32_t idx, QString name)
{

}

void t_Cuma_File::save_frag(QByteArray &b, QString name, uint32_t idx)
{

}
