#include "windows_proxy.h"

#include <Windows.h>
#include <Wininet.h>

void WindowsProxy::set(const QString &server, const QString &bypassList) {
    INTERNET_PER_CONN_OPTION_LIST connOptions;
    connOptions.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
    connOptions.pszConnection = NULL;
    connOptions.dwOptionCount = 3;  // Number of options to set
    connOptions.dwOptionError = 0;
    connOptions.pOptions = new INTERNET_PER_CONN_OPTION[3];

    // Set proxy server
    connOptions.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
    connOptions.pOptions[0].Value.dwValue = PROXY_TYPE_PROXY;
    connOptions.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
    connOptions.pOptions[1].Value.pszValue = (LPTSTR)server.utf16();

    // Set bypass list
    connOptions.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
    connOptions.pOptions[2].Value.pszValue = (LPTSTR)bypassList.utf16();

    DWORD bufferSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
    InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION,
                      &connOptions,bufferSize);

    delete[] connOptions.pOptions;
}

void WindowsProxy::clear() {
    INTERNET_PER_CONN_OPTION_LIST connOptions;
    connOptions.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
    connOptions.pszConnection = NULL;
    connOptions.dwOptionCount = 3;  // Number of options to set
    connOptions.dwOptionError = 0;
    connOptions.pOptions = new INTERNET_PER_CONN_OPTION[3];

    // Set proxy server to empty string to clear the proxy settings
    connOptions.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
    connOptions.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT;  // No proxy

    // Set proxy server to empty string
    connOptions.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
    connOptions.pOptions[1].Value.pszValue = const_cast<LPWSTR>(L"");

    // Set bypass list to empty string
    connOptions.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
    connOptions.pOptions[2].Value.pszValue = const_cast<LPWSTR>(L"");

    DWORD bufferSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
    InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION,
                      &connOptions, bufferSize);

    delete[] connOptions.pOptions;
}

bool WindowsProxy::isEnabled() {
    INTERNET_PER_CONN_OPTION_LIST connOptions;
    connOptions.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
    connOptions.pszConnection = NULL;
    connOptions.dwOptionCount = 1;  // Number of options to query
    connOptions.dwOptionError = 0;
    connOptions.pOptions = new INTERNET_PER_CONN_OPTION[1];

    // Query proxy flags
    connOptions.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;

    DWORD bufferSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
    if (InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION,
                            &connOptions, &bufferSize)) {
        bool proxyEnabled = (connOptions.pOptions[0].Value.dwValue &
                             PROXY_TYPE_PROXY) != 0;
        delete[] connOptions.pOptions;
        return proxyEnabled;
    } else {
        delete[] connOptions.pOptions;
        return false; // Failed to query proxy settings
    }
}
