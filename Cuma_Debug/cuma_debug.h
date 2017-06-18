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
    static bool show_debug;

};

#endif // CUMA_DEBUG_H
