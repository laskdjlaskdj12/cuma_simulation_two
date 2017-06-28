#include "cuma_debug.h"


Cuma_Debug::Cuma_Debug(QString Debug, uint line)
{

}

void Cuma_Debug::show_debug(bool b)
{

}

Cuma_Error::Cuma_Error(QString Error, uint line, uint32_t pid):m_Error(Error),m_line(line),m_pid(pid)
{
}

void Cuma_Error::show_error_string()
{

}

Cuma_Debug_protocol::Cuma_Debug_protocol(QJsonObject o, uint32_t pid):Cuma_Debug(o["Error_str"].toString())
{

}
