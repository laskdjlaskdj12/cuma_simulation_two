#include "cuma_debug.h"

Cuma_Debug::m_show_debug(false);

Cuma_Debug::Cuma_Debug(QString Debug)
{
    if(m_show_debug == true)
    {
        qDebug() <<"[Info] : " + Debug;
    }
}

Cuma_Debug::Cuma_Debug(QString Debug, uint line)
{
    if(m_show_debug == true)
    {
        qDebug() <<"[Info] : " + Debug + " line :"+ line;
    }
}

Cuma_Debug::Cuma_Debug(QJsonObject o, uint32_t pid)
{
    if(m_show_debug == true)
    {
        QJsonDocument doc(o);
        qDebug() <<"[Info] : " + doc.toJson() + " pid :"+ QString::number(pid);
    }

}

void Cuma_Debug::show_debug(bool b)
{
    m_show_debug = b;
}

bool Cuma_Debug::is_show_debug()
{
    return m_show_debug;
}

Cuma_Error::Cuma_Error(QString Error, uint line, uint32_t pid):m_Error(Error),m_line(line),m_pid(pid)
{

}

void Cuma_Error::show_error_string()
{
    qDebug()<<"[Error] : " + m_Error;
    qDebug()<<"[Error] : line : "+ m_line;
    qDebug()<<"[Error] : pid : " + m_pid;
}

Cuma_Debug_protocol::Cuma_Debug_protocol(QJsonObject o, uint32_t pid):Cuma_Debug(o["Error_str"].toString())
{

}
