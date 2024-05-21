#include "main_window.h"

#include <QApplication>
#include <QLocale>
#include <QMessageBox>
#include <QSharedMemory>
#include <QTimer>
#include <QTranslator>

#include <Windows.h>

#include "privilege_manager.h"
#include "settings_manager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setQuitOnLastWindowClosed(false);
    QCoreApplication::setOrganizationName("NextIn");
    QCoreApplication::setApplicationName("qsing-box");

    QSharedMemory sharedMemory("qsing-box");
    if (!sharedMemory.create(1)) {
        QMessageBox::warning(nullptr, QMessageBox::tr("Warning"),
                             QMessageBox::tr("qsing-box is already running.")
                             );
        return 1;
    }

    PrivilegeManager privilegeManager;
    SettingsManager settingsManager;
    if (settingsManager.runAsAdmin() && !privilegeManager.isRunningAsAdmin()) {
        QString appPath = QCoreApplication::applicationFilePath();

        if (!privilegeManager.runAsAdmin(appPath)) {
            if (privilegeManager.getLastError() == ERROR_CANCELLED) {
                // Rejected UAC prompt
                qDebug() << "Administrator permissions were denied.\n";
            } else {
                // Other errors cause privilege elevation to fail
                qDebug() << "Failed to launch as administrator.\n";
            }
        } else {
            // Successfully started administrator mode
            // and exited the current instance
            return 0;
        }
    }

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "qsing-box_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    app.installTranslator(&translator);

    MainWindow mainWindow;
    if (privilegeManager.isRunningAsAdmin()) {
        QString title = mainWindow.windowTitle() + QObject::tr(" (Administrator)");
        mainWindow.setWindowTitle(title);
    }

    bool isAutorun = false;
    for (int i = 1; i < argc; ++i) {
        if (QString(argv[i]) == "/autorun") {
            isAutorun = true;
            break;
        }
    }
    if (isAutorun) {
        QTimer::singleShot(3000, &mainWindow, &MainWindow::startProxy);
    } else {
        mainWindow.show();
    }

    return app.exec();
}
