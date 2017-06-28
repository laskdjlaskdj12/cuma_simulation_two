#ifndef CUMA_DEBUG_H
#define CUMA_DEBUG_H

#include <QJsonObject>
#include <QObject>

class Cuma_Debug
{
public:
    Cuma_Debug(QString Debug);
    Cuma_Debug(QString Debug, uint line);
    Cuma_Debug(QJsonObject o, uint32_t pid);
    static void show_debug(bool b);

protected:
    static bool is_show_debug();

private:
    static bool m_show_debug;

};

class Cuma_Debug_protocol: public Cuma_Debug
{
public:
    Cuma_Debug_protocol(QJsonObject o, uint32_t pid);
};

class Cuma_Error
{
public:
    Cuma_Error(QString Error, uint line, uint32_t pid = 0);

    void show_error_string();

private:
    QString m_Error;
    uint m_line;
    uint32_t m_pid;
};

#endif // CUMA_DEBUG_H
