#include "main_window.h"
#include "ui_main_window.h"

#include <QLabel>
#include <QMessageBox>
#include <QRegularExpression>
#include <QScrollBar>
#include <QString>

#include "about_dialog.h"
#include "ansi_color_text.h"
#include "settings_dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Remove the minimize and maximize buttons
    setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);
    resize(600, 450);
    setMaximumSize(720, 720);

    m_versionLabel = new QLabel(this);
    m_versionLabel->setText("v" + QString(PROJECT_VERSION));
    m_versionLabel->setIndent(8);
    ui->statusbar->addWidget(m_versionLabel);
    ui->statusLabel->setPixmap(QPixmap(":/images/status_disabled.png").
                               scaled(QSize(48, 48)));

    ui->stopButton->setEnabled(false);
    ui->outputEdit->setReadOnly(true);
    ui->outputEdit->setMaximumBlockCount(1000);

    m_proxyManager = new ProxyManager(this);
    connect(m_proxyManager, &ProxyManager::proxyProcessStateChanged, this,
            &MainWindow::changeProxy);
    connect(m_proxyManager, &ProxyManager::proxyProcessReadyReadStandardError, this,
            &MainWindow::displayProxyOutput);

    m_configManager = new ConfigManager(this);
    connect(m_configManager, &ConfigManager::configUpdated, this,
            &MainWindow::updateConfigList);
    connect(m_configManager, &ConfigManager::configChanged, this,
            &MainWindow::changeSelectedConfig);

    ui->configNameLabel->setWordWrap(true);
    // Load the config list on first startup
    updateConfigList();
    changeSelectedConfig();

    connect(ui->configListWidget, &QListWidget::currentRowChanged, this, &MainWindow::enableButton);
    if (ui->configListWidget->currentRow() == -1) {
        ui->editButton->setEnabled(false);
        ui->deleteButton->setEnabled(false);
    }

    m_trayIcon = new TrayIcon(this);
    connect(m_trayIcon, &TrayIcon::disableProxyActionTriggered, this, &MainWindow::stopProxy);
    connect(m_trayIcon, &TrayIcon::enableProxyActionTriggered, this, &MainWindow::startProxy);
    connect(m_trayIcon, &TrayIcon::restoreActionTriggered, this, &QWidget::showNormal);
    connect(m_trayIcon, &TrayIcon::iconActivated, this, &MainWindow::showMainWindow);
    connect(this, &MainWindow::proxyChanged, m_trayIcon, &TrayIcon::setMenuEnabled);
    m_trayIcon->show();
}

MainWindow::~MainWindow()
{
    stopProxy();
    delete ui;
}

void MainWindow::startProxy()
{
    m_proxyManager->startProxy();
    ui->outputEdit->clear();
}

void MainWindow::stopProxy()
{
    m_proxyManager->stopProxy();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!event->spontaneous() || !isVisible())
        return;
    if (m_trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void MainWindow::on_startButton_clicked()
{
    startProxy();
}

void MainWindow::on_stopButton_clicked()
{
    stopProxy();
}

void MainWindow::on_settingsButton_clicked()
{
    SettingsDialog settingsDialog(m_configManager, this);
    settingsDialog.exec();
}

void MainWindow::on_aboutButton_clicked()
{
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::on_addButton_clicked()
{
    m_configManager->addConfig();
}

void MainWindow::on_editButton_clicked()
{

    int index = ui->configListWidget->currentRow();
    if (index >= 0 && m_configManager->configCount() > 0) {
        m_configManager->editConfig(index);
    }
}

void MainWindow::on_deleteButton_clicked()
{

    int index = ui->configListWidget->currentRow();
    if (index >= 0 && m_configManager->configCount() > 0) {
        int ret = QMessageBox::question(this, tr("Confirm"),
                                        tr("Do you want to delete it?"));
        if (ret == QMessageBox::Yes) {
            m_configManager->removeConfig(index);
        }
    }
}

void MainWindow::on_importButton_clicked()
{
    m_configManager->importConfig();
}

void MainWindow::on_switchButton_clicked()
{
    int index = ui->configListWidget->currentRow();
    if (index >= 0) {
        m_configManager->switchConfig(index);
    }
}

void MainWindow::enableButton(int currentRow)
{
    if (currentRow != -1) {
        ui->editButton->setEnabled(true);
        ui->deleteButton->setEnabled(true);
    } else {
        ui->editButton->setEnabled(false);
        ui->deleteButton->setEnabled(false);
    }
}

void MainWindow::displayProxyOutput()
{
    QByteArray outputData = m_proxyManager->readProxyProcessAllStandardError();
    QString outputText = QString::fromUtf8(outputData);
    // Parsing ANSI colors and dispalys
    AnsiColorText::appendAnsiColorText(ui->outputEdit, outputText);
    // Scroll to latest content
    QScrollBar *scrollBar = ui->outputEdit->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void MainWindow::updateConfigList()
{
    ui->configListWidget->clear();
    if (m_configManager->configCount() > 0) {
        QStringList configNames = m_configManager->configNames();
        for (int i = 0; i < configNames.size(); i++) {
            QString itemText = QString("%1. %2").arg(i + 1).arg(configNames.at(i));
            ui->configListWidget->addItem(itemText);
        }
        ui->configListWidget->setCurrentRow(m_configManager->configIndex());
    }
}

void MainWindow::changeProxy(int newState)
{
    if (m_proxyManager->proxyProcessState() == QProcess::Running)
    {
        m_trayIcon->setIcon(QIcon(":/images/app_enable_proxy.ico"));
        setWindowIcon(QIcon(":/images/app_enable_proxy.ico"));
        ui->statusLabel->setPixmap(QPixmap(":/images/status_enabled.png").
                                   scaled(QSize(48, 48)));
        ui->startButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
        emit proxyChanged(true);
    } else if (newState == QProcess::NotRunning) {
        m_trayIcon->setIcon(QIcon(":/images/app.ico"));
        setWindowIcon(QIcon(":/images/app.ico"));
        ui->statusLabel->setPixmap(QPixmap(":/images/status_disabled.png").
                                   scaled(QSize(48, 48)));
        ui->startButton->setEnabled(true);
        ui->stopButton->setEnabled(false);
        emit proxyChanged(false);
        m_proxyManager->clearSystemProxy();
    }
}

void MainWindow::showMainWindow(int reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::MiddleClick:
        this->show();
        break;
    default:
        ;
    }
}

void MainWindow::changeSelectedConfig()
{
    if (m_configManager->configCount() == 0) {
        ui->configNameLabel->setText(tr("<Empty config>"));
        if (m_proxyManager->proxyProcessState() == QProcess::Running)
        {
            stopProxy();
        }
    } else {
        QString name = m_configManager->configName();
        int index = m_configManager->configIndex();
        ui->configNameLabel->setText(QString("%1. %2").arg(index + 1).arg(name));
        m_proxyManager->setConfigFilePath(m_configManager->configFilePath());
        if (m_proxyManager->proxyProcessState() == QProcess::Running)
        {
            stopProxy();
            startProxy();
        }
    }
}
