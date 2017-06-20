#ifndef CUMA_DEBUG_H
#define CUMA_DEBUG_H

#include <QObject>

class Cuma_Debug
{
public:
    Cuma_Debug(QString Debug);
    Cuma_Debug(QString Debug, uint line);

    static void show_debug(bool b);

private:
    static bool m_show_debug;

};

class Cuma_Error
{
public:
    Cuma_Error(QString Error, uint line, uint32_t pid);

    void show_error_string();

private:
    QString m_Error;
    uint m_line;
    uint32_t m_pid;
};

#endif // CUMA_DEBUG_H
