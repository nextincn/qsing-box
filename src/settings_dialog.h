#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>

#include "config_manager.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(ConfigManager *configManager, QWidget *parent = nullptr);
    ~SettingsDialog();

private slots:
    void on_clearDataButton_clicked();
    void on_autoRunCheckBox_clicked(bool checked);
    void on_runAsAdminCheckBox_clicked(bool checked);

private:
    Ui::SettingsDialog *ui;
    ConfigManager *m_configManager;
    bool m_autoRun;
    bool m_runAsAdmin;
};

#endif // SETTINGS_DIALOG_H
