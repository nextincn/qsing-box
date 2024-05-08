#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QProcess>

#include "config_manager.h"
#include "proxy_manager.h"
#include "tray_icon.h"

QT_BEGIN_NAMESPACE
class QLabel;

namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void startProxy();
    void stopProxy();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void proxyChanged(bool enabled);

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_settingsButton_clicked();
    void on_aboutButton_clicked();
    void on_addButton_clicked();
    void on_editButton_clicked();
    void on_deleteButton_clicked();
    void on_importButton_clicked();
    void on_switchButton_clicked();

    void enableButton(int currentRow);

    void displayProxyOutput();
    void updateConfigList();

    // when the state of the proxy has changed,
    // then execute this slot function
    void changeProxy(int newState);

    // Click on the system tray, then execute this slot function
    void showMainWindow(int reason);
    void changeSelectedConfig();

private:
    Ui::MainWindow *ui;
    QLabel *m_versionLabel;

    TrayIcon *m_trayIcon;
    ConfigManager *m_configManager;
    ProxyManager *m_proxyManager;
};

#endif // MAIN_WINDOW_H
