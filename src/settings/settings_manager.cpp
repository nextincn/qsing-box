#include "settings_manager.h"

#include <QCoreApplication>
#include <QDir>
#include <QSettings>

SettingsManager::SettingsManager(QObject *parent)
    : QObject{parent}
{}

QString SettingsManager::lastOpenedFilePath()
{
    QSettings settings;
    return settings.value("lastOpenedFilePath").toString();
}

void SettingsManager::setLastOpenedFilePath(const QString &filePath)
{
    QSettings settings;
    settings.setValue("lastOpenedFilePath", filePath);
}

int SettingsManager::configIndex()
{
    QSettings settings;
    return settings.value("configIndex").toInt();
}

void SettingsManager::setConfigIndex(int index)
{
    QSettings settings;
    settings.setValue("configIndex", index);
}

bool SettingsManager::autoRun()
{
    QSettings settings;
    return settings.value("autoRun", false).toBool();
}

void SettingsManager::setAutoRun(bool checked)
{
    QSettings settings;
    settings.setValue("autoRun", checked);
}

void SettingsManager::setAppAutoRun(bool enabled)
{
    QString appName = QCoreApplication::applicationName();
    QString appPath = QCoreApplication::applicationFilePath();
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                              QSettings::NativeFormat);

    if (enabled == true) {
        settings.setValue(appName, "\"" + QDir::toNativeSeparators(appPath) + "\" /autorun");
    } else {
        settings.remove(appName);
    }
}

bool SettingsManager::runAsAdmin()
{
    QSettings settings;
    return settings.value("runAsAdmin", false).toBool();
}

void SettingsManager::setRunAsAdmin(bool checked)
{
    QSettings settings;
    settings.setValue("runAsAdmin", checked);
}

void SettingsManager::removeConfig()
{
    QSettings settings;
    settings.beginWriteArray("Config");
    settings.remove("");
    settings.endArray();
}

void SettingsManager::clearAllSettings()
{
    QSettings settings;
    settings.clear();
}
