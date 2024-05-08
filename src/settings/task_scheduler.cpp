#include "task_scheduler.h"

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QStandardPaths>

#include "privilege_manager.h"

TaskScheduler::TaskScheduler(QObject *parent)
    : QObject{parent}
{}

bool TaskScheduler::createTask()
{
    QFile xmlFile(":/xml/task.xml");
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open xml file.";
        return false;
    }

    QTextStream inputStream(&xmlFile);
    inputStream.setEncoding(QStringConverter::Utf16);
    inputStream.setGenerateByteOrderMark(true);
    QString xmlContent = inputStream.readAll();
    xmlFile.close();

    QString appPath = QCoreApplication::applicationFilePath();
    xmlContent.replace("$PROGRAM_PATH$", appPath);

    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QFile outputFile(tempDir + "/qsing-box.xml");
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open output file.";
        return false;
    }

    QTextStream outputStream(&outputFile);
    outputStream.setEncoding(QStringConverter::Utf16);
    outputStream.setGenerateByteOrderMark(true);
    outputStream << xmlContent;
    outputFile.close();

    QString taskName = QCoreApplication::applicationName();
    QString commandLine = QString("/create /xml \"%1\" /tn \"%2\"")
                              .arg(QFileInfo(outputFile).filePath()).arg(taskName);

    PrivilegeManager privilegeManager;
    return privilegeManager.runAsAdmin("schtasks.exe", commandLine);
}

bool TaskScheduler::removeTask()
{
    QString taskName = QCoreApplication::applicationName();
    QString commandLine = "/delete /tn \"" +taskName + "\" /f";
    PrivilegeManager privilegeManager;
    return privilegeManager.runAsAdmin("schtasks.exe", commandLine);
}
