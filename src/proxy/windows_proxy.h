#ifndef WINDOWS_PROXY_H
#define WINDOWS_PROXY_H

#include <QString>

class WindowsProxy
{
public:
    static void set(const QString &server, const QString &bypassList);
    static void clear();
    static bool isEnabled();
};

#endif // WINDOWS_PROXY_H
