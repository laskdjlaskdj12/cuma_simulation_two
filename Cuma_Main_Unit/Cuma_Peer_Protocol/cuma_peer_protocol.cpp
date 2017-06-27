#include "cuma_peer_protocol.h"


QJsonObject cuma_protocol::basic_command_protocol(uint32_t From_Pid)
{
    QJsonObject recv_obj;

    recv_obj["From"] = From_Pid;

    return recv_obj;
}

QJsonObject cuma_protocol::basic_protocol(uint32_t From_Pid)
{
    QJsonObject recv_obj;

    recv_obj["From"] = From_Pid;

    return recv_obj;
}

QJsonObject cuma_protocol::req_unit_command_protocol(QString command)
{
    QJsonObject command_protocol;
    //만약 커맨드가 set_file_name일때
    if(command == "set_file_name" || command == "set_bypass_count")
    {
        Cuma_Debug("this command is not used in this command");
        Cuma_Debug("try to use cuma_protocol::req_unit_command_protocol(QString command, uint32_t count)");
        return nullptr;
    }

    if(command == "ping")
    {
        command_protocol["command_ping_test"] = true;
    }

    if(command == "spread")
    {
        command_protocol["command_spread_test"] = true;
    }

    if(command == "request")
    {
        command_protocol["command_rq_file"] = true;
    }

    if(command == "track")
    {
        command_protocol["command_trace_pass"] = true;
    }

    else
    {
        command_protocol = nullptr;
    }

    return command_protocol;
}

QJsonObject cuma_protocol::req_unit_command_protocol(QString command, uint32_t count)
{
    QJsonObject command_protocol;

    if(command == "set_file_name")
    {
        command_protocol["command_set_unit_bypass_count"] = count;
    }
    else
    {
        command_protocol = nullptr;
    }
    return command_protocol;
}

QJsonObject cuma_protocol::req_unit_command_protocol(QString command, QString name)
{
    QJsonObject command_protocol;

    if(command == "set_file_name")
    {
        command_protocol["command_set_file_name"] = name;
    }
    else
    {
        command_protocol = nullptr;
    }
    return command_protocol;
}

QJsonObject cuma_protocol::req_ping_protocol(uint32_t unit_id, bool reply)
{
    QJsonObject recv_obj = basic_protocol(unit_id);

    recv_obj["process"] = "ping";
    recv_obj["reply"] = is_return;
    recv_obj["From"] = unit_id;

    return recv_obj;
}

QJsonObject cuma_protocol::req_is_file_exsist_protocol(uint32_t file_frag_index, uint32_t unit_id)
{
    QJsonObject recv_obj = basic_protocol(unit_id);

    recv_obj["process"] = "check_file";
    recv_obj["index"] = file_frag_index;

    return recv_obj;
}

QJsonObject cuma_protocol::req_is_file_exsist_protocol(QString f_name, uint32_t unit_id, bool req_file_index = true)
{

    QJsonObject recv_obj = basic_protocol(unit_id);

    recv_obj["process"] = "check_file";
    recv_obj["index"] = file_frag_index;
    recv_obj["reply"] = req_file_index;
    recv_obj["file_name"] = f_name;
    recv_obj["file_index"] = unit_id;
    recv_obj["file_req"] = req_file_index;
    return recv_obj;
}

QJsonObject cuma_protocol::req_file_binary_save_protocol(QJsonObject file_binary, uint32_t unit_id)
{
    QJsonObject recv_obj = basic_protocol(unit_id);

    recv_obj["process"] = "save";
    recv_obj["index"] = file_frag_index;
    recv_obj["reply"] = true;
    recv_obj["file_frag"] = send_frag_binary["file_frag"];
    recv_obj["file_name"] = send_frag_binary["file_name"];
    recv_obj["file_index"] = send_frag_binary["file_index"];
    recv_obj["file_info_block"] = send_frag_binary["file_info_block"];

    return recv_obj;
}

QJsonObject cuma_protocol::req_file_binary_read_protocol(QString binary_name, uint32_t file_frag_index, uint32_t unit_id)
{
    QJsonObject recv_obj = basic_protocol(unit_id);

    recv_obj["process"] = "read";
    recv_obj["file_name"] = binary_name;
    recv_obj["index"] = file_frag_index;
    return recv_obj;
}

QJsonObject cuma_protocol::reply_ping_protocol(uint32_t From_uid, bool is_return = false)
{
    QJsonObject recv_obj = basic_protocol(From_uid);

    recv_obj["process"] = "check_file";
    recv_obj["index"] = file_frag_index;
    recv_obj["reply"] = is_return;
    return recv_obj;
}

QJsonObject cuma_protocol::reply_is_file_exsist_protocol(uint32_t From_uid,, uint32_t file_frag_index, bool is_exsist)
{
    QJsonObject recv_obj = basic_protocol(From_uid);

    recv_obj["process"] = "ping";
    recv_obj["file_index"] = file_frag_index;
    recv_obj["is_exsist"] = is_exsist;
    recv_obj["reply"] = true;

    return recv_obj;
}

QJsonObject cuma_protocol::reply_file_binary_save_protocol(uint32_t From_uid, QString file_frag_name, uint32_t frag_index)
{
    QJsonObject recv_obj = basic_command_protocol(From_uid);

    recv_obj["process"] = "save";
    recv_obj["file_name"] = file_frag_name;
    recv_obj["file_index"] = frag_index;
    recv_obj["reply"] = true;

    return recv_obj;
}

QJsonObject cuma_protocol::reply_file_binary_read_protocol(uint32_t From_uid, QString file_frag_name, uint32_t file_frag_index, QByteArray binary )
{
    QJsonObject recv_obj = basic_command_protocol(From_uid);

    recv_obj["process"] = "read";
    recv_obj["file_frag"] = binary;
    recv_obj["file_name"] = file_frag_name;
    recv_obj["file_index"] = file_frag_index;
    recv_obj["index"] = file_frag_index;
    recv_obj["reply"] = true;

    return recv_obj;
}
