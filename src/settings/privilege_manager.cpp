#include "privilege_manager.h"

#include <QCoreApplication>
#include <QProcess>
#include <QSharedMemory>

#include <Windows.h>

PrivilegeManager::PrivilegeManager(QObject *parent)
    : QObject{parent}
{}

bool PrivilegeManager::isRunningAsAdmin()
{
    BOOL fIsElevated = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation;
        DWORD dwSize;
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
            fIsElevated = elevation.TokenIsElevated;
        }
    }
    if (hToken) {
        CloseHandle(hToken);
    }
    return fIsElevated;
}

quint32 PrivilegeManager::getLastError()
{
    DWORD dwError = GetLastError();
    quint32 qtError = static_cast<quint32>(dwError);
    return qtError;
}

bool PrivilegeManager::runAsAdmin(const QString &programPath, const QString &parameters)
{
    SHELLEXECUTEINFO shellExecuteInfo = { sizeof(SHELLEXECUTEINFO) };
    shellExecuteInfo.lpFile = reinterpret_cast<LPCWSTR>(programPath.utf16());
    shellExecuteInfo.lpParameters = parameters.isEmpty() ? nullptr : reinterpret_cast<LPCWSTR>(parameters.utf16());
    shellExecuteInfo.lpVerb = L"runas";
    shellExecuteInfo.nShow = SW_NORMAL;
    return ShellExecuteEx(&shellExecuteInfo);
}

void PrivilegeManager::restartProgram()
{
    QString appPath = QCoreApplication::applicationFilePath();

    qApp->quit();
    QProcess::startDetached(appPath);
}
