#include "config_manager.h"

#include <QCoreApplication>
#include <QFile>
#include <QFileDialog>
#include <QListWidget>
#include <QSettings>
#include <QStandardPaths>

#include "settings_manager.h"

ConfigManager::ConfigManager(QObject *parent)
    : QObject{parent}
{
    m_configEditor = new ConfigEditor();
    connect(m_configEditor, &ConfigEditor::configFileSaved, this, &ConfigManager::appendConfigList);
    connect(m_configEditor, &ConfigEditor::editedConfigFileSaved, this, &ConfigManager::updateConfigList);
    getConfigFromSettings();

    SettingsManager settingsManager;
    m_configIndex = settingsManager.configIndex();
}

ConfigManager::~ConfigManager()
{
    delete m_configEditor;
}

void ConfigManager::addConfig()
{
    m_configEditor->addFile();
    if (m_configList.count() == 1)
        emit configChanged();
}

void ConfigManager::importConfig()
{
    m_configEditor->openFile();
    if (m_configList.count() == 1)
        emit configChanged();
}

void ConfigManager::editConfig(int index)
{
    if (index >= 0 && index < m_configList.size()) {
        QString filePath = m_configList.at(index).filePath();
        QString name = m_configList.at(index).name();
        m_configEditor->openFile(index, filePath, name);
    }
}

void ConfigManager::removeConfig(int index)
{
    if (index >= 0 && index < m_configList.size()) {
        QString filePath = m_configList.at(index).filePath();
        QFile(filePath).remove();
        m_configList.remove(index);
        saveConfigToSettings();
        int count = m_configList.count();
        // Already deleted, get new configuration quantity
        if (count != 0) {
            // The previous item is deleted,
            // and the selected configuration is moved forward by one item.
            if (index < m_configIndex) {
                switchConfig(m_configIndex - 1);
            }
            // Deleting the latter item,
            // have no impact on the selected configuration
            if (index > m_configIndex) {
            }
            // Delete the selected configuration,
            // need to switch to other configurations
            if (index == m_configIndex) {
                // The deleted item is not the last one
                if (index != count)
                {
                    switchConfig(m_configIndex);
                } else { // Deleted the last one
                    switchConfig(m_configIndex - 1);
                }
            }
        } else {
            // The configuration has been cleared,
            // need other operations
            emit configChanged();
        }
        emit configUpdated();
    }
}

void ConfigManager::switchConfig(int index)
{
    if (index >= 0 && index < m_configList.size()) {
        SettingsManager settingsManager;
        settingsManager.setConfigIndex(index);
        m_configIndex = index;

        emit configChanged();
    }
}

QStringList ConfigManager::configNames() const
{
    QStringList names;
    for (const auto &config : m_configList) {
        names.append(config.name());
    }
    return names;
}

QString ConfigManager::configFilePath() const
{
    if (!m_configList.isEmpty()){
        return m_configList.at(m_configIndex).filePath();
    } else {
        return QString();
    }
}

QString ConfigManager::configName() const
{
    if (!m_configList.isEmpty()){
        return m_configList.at(m_configIndex).name();
    } else {
        return QString();
    }
}

int ConfigManager::configIndex() const
{
    return m_configIndex;
}

int ConfigManager::configCount() const
{
    return m_configList.count();
}

void ConfigManager::deleteAllConfig()
{
    m_configList.clear();
    m_configIndex = 0;

    QString localPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    QString directory = QString("%1/%2/config").arg(localPath)
                            .arg(QCoreApplication::applicationName());
    QDir dir(directory);
    if (dir.exists()) {
        dir.removeRecursively();
    }

    SettingsManager settingsManager;
    settingsManager.setConfigIndex(m_configIndex);
    settingsManager.removeConfig();

    emit configUpdated();
    emit configChanged();

}

void ConfigManager::appendConfigList(const QString &filePath, const QString &name)
{
    m_configList.append(Config{filePath, name});
    saveConfigToSettings();
    emit configUpdated();
}

void ConfigManager::updateConfigList(int index, const QString &filePath, const QString &name)
{
    if (index >= 0 && index < m_configList.size()) {
        m_configList[index] = Config{filePath, name};
    }
    saveConfigToSettings();
    emit configUpdated();
}

void ConfigManager::getConfigFromSettings()
{
    QSettings settings;
    int size = settings.beginReadArray("Config");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString filePath = settings.value("filePath").toString();
        QString name = settings.value("name").toString();
        m_configList.append(Config{filePath, name});
    }
    settings.endArray();

    emit configUpdated();
}

void ConfigManager::saveConfigToSettings()
{
    QSettings settings;
    settings.beginWriteArray("Config");
    settings.remove("");
    for (int i = 0; i < m_configList.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("filePath", m_configList.at(i).filePath());
        settings.setValue("name", m_configList.at(i).name());
    }
    settings.endArray();
}
