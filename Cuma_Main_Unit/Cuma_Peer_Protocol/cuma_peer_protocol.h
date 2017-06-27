#ifndef CUMA_PEER_PROTOCOL_H
#define CUMA_PEER_PROTOCOL_H

#include <QObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonDocument>

class cuma_protocol{

public:
    static QJsonObject basic_command_protocol(uint32_t From_Pid);
    static QJsonObject basic_protocol(uint32_t From_Pid);
public:
    static QJsonObject req_unit_command_protocol(QString command);
    static QJsonObject req_unit_command_protocol(QString command, uint32_t count);
    static QJsonObject req_unit_command_protocol(QString command, QString name);

public:
    static QJsonObject req_ping_protocol(uint32_t unit_id, bool reply = false);
    static QJsonObject req_is_file_exsist_protocol(uint32_t file_frag_index, uint32_t unit_id);
    static QJsonObject req_is_file_exsist_protocol(QString f_name, uint32_t unit_id, bool req_file_index = true);
    static QJsonObject req_file_binary_save_protocol(QJsonObject file_binary, uint32_t unit_id);
    static QJsonObject req_file_binary_read_protocol(QString binary_name, uint32_t file_frag_index, uint32_t unit_id );

public:
    static QJsonObject reply_ping_protocol(uint32_t From_uid, bool is_return = false);
    static QJsonObject reply_is_file_exsist_protocol(uint32_t From_uid, uint32_t file_frag_index, bool is_exsist);
    static QJsonObject reply_file_binary_save_protocol(uint32_t From_uid, QString file_frag_name, uint32_t frag_index);
    static QJsonObject reply_file_binary_read_protocol(uint32_t From_uid, QString file_frag_name, uint32_t file_frag_index, QByteArray binary );
};

#endif // CUMA_PEER_PROTOCOL_H
