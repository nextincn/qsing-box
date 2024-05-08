#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include <QMessageBox>

#include "privilege_manager.h"
#include "settings_manager.h"
#include "task_scheduler.h"

SettingsDialog::SettingsDialog(ConfigManager *configManager, QWidget *parent)
    : m_configManager(configManager)
    , QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setFixedSize(400, 200);

    SettingsManager settingsManager;
    m_autoRun = settingsManager.autoRun();
    m_runAsAdmin = settingsManager.runAsAdmin();
    ui->autoRunCheckBox->setChecked(m_autoRun);
    ui->runAsAdminCheckBox->setChecked(m_runAsAdmin);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_clearDataButton_clicked()
{
    int ret = QMessageBox::warning(this,
                                   tr("Warning"),
                                   tr("All configuration files will be cleared.\n"
                                      "Do you want to continue?"),
                                   QMessageBox::Ok | QMessageBox::No
                                   );
    if (ret == QMessageBox::Ok) {
        m_configManager->deleteAllConfig();
    }
}

void SettingsDialog::on_autoRunCheckBox_clicked(bool checked)
{
    TaskScheduler taskScheduler;
    SettingsManager settingsManager;
    PrivilegeManager privilegeManager;

    m_autoRun = checked;
    settingsManager.setAutoRun(m_autoRun);
    if (!m_runAsAdmin) {
        // Not run as admin
        settingsManager.setAppAutoRun(m_autoRun);
    } else {
        // Run as admin
        if (!(m_autoRun ? taskScheduler.createTask() : taskScheduler.removeTask())) {
            // Refuse to create or remove the task
            ui->autoRunCheckBox->setChecked(!checked);
            m_autoRun = !checked;
            settingsManager.setAutoRun(m_autoRun);
        }
    }
}

void SettingsDialog::on_runAsAdminCheckBox_clicked(bool checked)
{
    TaskScheduler taskScheduler;
    SettingsManager settingsManager;
    PrivilegeManager privilegeManager;

    m_runAsAdmin = checked;
    settingsManager.setRunAsAdmin(m_runAsAdmin);
    if (!m_autoRun) {
        // Not run automatically
        int ret = QMessageBox::information(this,
                                           tr("Information"),
                                           tr("Need to restart the program.\n"
                                              "Do you want to restart the program?")
                                           );
        if (ret == QMessageBox::Ok) {
            privilegeManager.restartProgram();
        }
    } else {
        // Run automatically
        settingsManager.setAppAutoRun(!checked);
        if (m_runAsAdmin ? taskScheduler.createTask() : taskScheduler.removeTask()) {
            // Create or remove the task successfully
            // privilegeManager.isRunningAsAdmin()
            int ret = QMessageBox::information(this,
                                               tr("Information"),
                                               tr("Need to restart the program.\n"
                                                  "Do you want to restart the program?")
                                               );
            if (ret == QMessageBox::Ok) {
                privilegeManager.restartProgram();
            }
        } else {
            // Refuse to create or remove the task
            ui->runAsAdminCheckBox->setChecked(!checked);
            m_runAsAdmin = !checked;
            settingsManager.setRunAsAdmin(m_runAsAdmin);
        }
    }
}
