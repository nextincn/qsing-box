#include "config_editor.h"
#include <ui_config_editor.h>

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

#include "settings_manager.h"

ConfigEditor::ConfigEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfigEditor)
{
    ui->setupUi(this);
    resize(600, 450);
    setMaximumSize(720, 720);

}

ConfigEditor::~ConfigEditor()
{
    delete ui;
}

void ConfigEditor::addFile()
{
    m_editMode = EditMode::AddNewConfig;
    ui->titleEdit->setText(QString());
    ui->contentEdit->setPlainText(QString());
    exec();
}

void ConfigEditor::openFile()
{
    m_editMode = EditMode::ImportConfig;
    SettingsManager settingsManager;
    QString lastOpenedFilePath = settingsManager.lastOpenedFilePath();
    if (lastOpenedFilePath.isEmpty()) {
        lastOpenedFilePath = QDir::homePath();
    }
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Select JSON file"),
                                                    lastOpenedFilePath,
                                                    tr("JSON File (*.json)")
                                                    );
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, tr("Error"),
                                 tr("Import config failed.")
                                 );
            return;
        }
        QTextStream textStream(&file);
        QString fileContent = textStream.readAll();
        file.close();
        QString fileBaseName = QFileInfo(filePath).baseName();
        ui->titleEdit->setText(fileBaseName);
        ui->contentEdit->setPlainText(fileContent);
        settingsManager.setLastOpenedFilePath(QFileInfo(filePath).absolutePath());
        exec();
    }
}

void ConfigEditor::openFile(int index, const QString &filePath, const QString &title)
{
    m_editMode = EditMode::EditConfig;
    m_configFileIndex = index;
    m_configFilePath = filePath;
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.exists()) {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("The configuration file does not exist or has been deleted.")
                                 );
            return;
        }
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, tr("Error"),
                                 tr("Open config failed.")
                                 );
            return;
        }
        QTextStream textStream(&file);
        QString fileContent = textStream.readAll();
        file.close();
        ui->titleEdit->setText(title);
        ui->contentEdit->setPlainText(fileContent);
        exec();
    }
}

void ConfigEditor::saveConfigFile(const QString &configContent)
{
    QString directory = QString(QCoreApplication::applicationDirPath() + "/config");
    QDir dir;
    // If the directory does not exist, create the directory
    if (!dir.exists(directory)) {
        dir.mkpath(directory);
    }

    QString filePath;
    if (m_editMode == EditMode::EditConfig) {
        filePath = m_configFilePath;
    } else {
        filePath = QString(directory + "/" + generateFileName());
    }
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream textStream(&file);
        textStream << configContent;
        file.close();
    }

    if (m_editMode == EditMode::EditConfig) {
        emit editedConfigFileSaved(m_configFileIndex, filePath, ui->titleEdit->text());
    } else {
        emit configFileSaved(QFileInfo(file).absoluteFilePath(), ui->titleEdit->text());
    }
}

QString ConfigEditor::generateFileName()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString timestamp = QString::number(currentDateTime.toMSecsSinceEpoch());
    QString fileName = "config_" + timestamp + ".json";
    return fileName;
}

void ConfigEditor::on_saveButton_clicked()
{
    if (ui->titleEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("The title can not be blank.")
                             );
        return;
    }
    saveConfigFile(ui->contentEdit->toPlainText());
    close();
}

void ConfigEditor::on_cancelButton_clicked()
{
    close();
}

