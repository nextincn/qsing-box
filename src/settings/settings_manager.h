#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <QObject>

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    explicit SettingsManager(QObject *parent = nullptr);

    QString lastOpenedFilePath();
    void setLastOpenedFilePath(const QString &filePath);

    int configIndex();
    void setConfigIndex(int index);

    bool autoRun();
    void setAutoRun(bool checked);
    void setAppAutoRun(bool enabled);

    bool runAsAdmin();
    void setRunAsAdmin(bool checked);

    void removeConfig();
    void clearAllSettings();
};

#endif // SETTINGS_MANAGER_H
