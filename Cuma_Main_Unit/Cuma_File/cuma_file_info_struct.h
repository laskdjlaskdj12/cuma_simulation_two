#ifndef CUMA_FILE_INFO_STRUCT_H
#define CUMA_FILE_INFO_STRUCT_H

#include <QObject>

struct Cuma_File_Info_Block
{
    QString f_Name;
    uint32_t f_Binary_Byte;
    uint32_t f_Frag_count;
    uint32_t f_Frag_byte;
    uint32_t Block_size;
};

#endif // CUMA_FILE_INFO_STRUCT_H
