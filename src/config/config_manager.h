#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <QObject>

#include "config.h"
#include "config_editor.h"

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = nullptr);
    ~ConfigManager();

    void addConfig();
    void editConfig(int index);
    void importConfig();
    void removeConfig(int index);
    void switchConfig(int index);

    QStringList configNames() const;
    QString configFilePath() const;
    QString configName() const;
    int configIndex() const;
    int configCount() const;

    void deleteAllConfig();

signals:
    void configUpdated();
    void configChanged();

private slots:
    void appendConfigList(const QString &filePath, const QString &name);
    void updateConfigList(int index, const QString &filePath, const QString &name);

private:
    // Read config list from registry
    void getConfigFromSettings();
    // Write config list to registry
    void saveConfigToSettings();

    int m_configIndex;
    QList<Config> m_configList;
    ConfigEditor *m_configEditor;
};

#endif // CONFIG_MANAGER_H
