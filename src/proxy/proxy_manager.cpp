#include "proxy_manager.h"

#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>

#include "windows_proxy.h"

ProxyManager::ProxyManager(QObject *parent)
    : QObject{parent}
{
    m_proxyProcess = new QProcess(this);
}

void ProxyManager::startProxy()
{
    QString program = QCoreApplication::applicationDirPath() + "/sing-box.exe";
    QFile file(program);
    if (!file.exists()) {
        QMessageBox::warning(nullptr, tr("Warning"),
                             tr("Can not find sing-box core!\n"
                                "Please place \"sing-box.exe\" in\n") + QCoreApplication::applicationDirPath()
                             );
    } else {
        if (m_configFilePath.isEmpty()) {
            QMessageBox::warning(nullptr, tr("Warning"),
                                 tr("The current configuration is empty!")
                                 );
        } else if (!QFile(m_configFilePath).exists()) {
            QMessageBox::warning(nullptr, tr("Warning"),
                                 tr("The current configuration is missing!")
                                 );
        } else {
            QStringList arguments;
            arguments << "run" << "-c" << m_configFilePath << "-D" << QCoreApplication::applicationDirPath();
            m_proxyProcess->start(program, arguments);
            connect(m_proxyProcess, &QProcess::stateChanged, this,
                    &ProxyManager::emitProxyProcessStateChanged);
            connect(m_proxyProcess, &QProcess::readyReadStandardError, this,
                    &ProxyManager::emitProxyProcessReadyReadStandardError);
            emitProxyProcessStateChanged(QProcess::Running);
        }
    }
}

void ProxyManager::stopProxy()
{
    if (m_proxyProcess->state() == QProcess::Running)
    {
        m_proxyProcess->kill();
        m_proxyProcess->waitForFinished();
    }
}

void ProxyManager::clearSystemProxy()
{
    WindowsProxy::clear();
}

bool ProxyManager::isSystemProxyEnabled() const
{
    return WindowsProxy::isEnabled();
}

QByteArray ProxyManager::readProxyProcessAllStandardError()
{
    return m_proxyProcess->readAllStandardError();
}

int ProxyManager::proxyProcessState() const
{
    return m_proxyProcess->state();
}

void ProxyManager::setConfigFilePath(const QString &filePath)
{
    m_configFilePath = filePath;
}

void ProxyManager::emitProxyProcessStateChanged(int newState)
{

    emit proxyProcessStateChanged(newState);
}

void ProxyManager::emitProxyProcessReadyReadStandardError()
{
    emit proxyProcessReadyReadStandardError();
}
