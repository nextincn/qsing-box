#include "about_dialog.h"
#include "ui_about_dialog.h"

#include <QFile>
#include <QScrollBar>
#include <QTimer>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->imageLabel->setPixmap(QPixmap(":/images/qt_logo.png").scaledToWidth(72));
    ui->imageLabel->setAlignment(Qt::AlignHCenter);

    ui->titleLabel->setText(QString("qsing-box v%1").arg(PROJECT_VERSION));
    ui->titleLabel->setFont(QFont("Arial", 14, QFont::Bold));

    ui->versionLabel->setText(tr("Based on Qt") + QString(" %1 %2 %3").arg( QT_VERSION_STR)
                                  .arg(COMPILER_ID).arg(COMPILER_VERSION));

    ui->buildLabel->setText(tr("Built on ") + BUILD_TIME);

    ui->introduceLabel->setText(tr("qsing-box is a Windows GUI client for sing-box.\n"
                                "It is developed using Qt C++."));
    ui->introduceLabel->setWordWrap(true);

    QString link1 = QString("https://github.com/SagerNet/sing-box");
    QString link2 = QString("https://github.com/nextincn/qsing-box");
    ui->projectLabel->setText("sing-box"
                              + QString(" &nbsp;&nbsp;<a href=\"%1\">%1</a><br>").arg(link1)
                              + "qsing-box"
                              + QString(" <a href=\"%1\">%1</a>").arg(link2)
                              );
    ui->projectLabel->setOpenExternalLinks(true);
    ui->projectLabel->setWordWrap(true);

    ui->copyrightLabel->setText(tr("Copyright 2024 qsing-box. All rights reserved."));
    ui->copyrightLabel->setWordWrap(true);

    QString Gplv3Link = QString("https://www.gnu.org/licenses/gpl-3.0.html");
    ui->licenseLabel->setText(tr("Open source license ")
                              + QString("<a href=\"%1\">GPLv3</a>").arg(Gplv3Link));
    ui->licenseLabel->setOpenExternalLinks(true);

    QFile file(":/texts/gpl-3.0.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        ui->licenseEdit->appendPlainText(line);
    }
    ui->licenseEdit->setReadOnly(true);
    ui->licenseEdit->setWordWrapMode(QTextOption::NoWrap);

    // Setting scrollbar position directly without using QTimer will not take effect.
    // Use QTimer to set scrollbar position later.
    QTimer::singleShot(0, ui->licenseEdit, [&]() {
        ui->licenseEdit->verticalScrollBar()->setValue(0);
    });

    setFixedSize(500, 350);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
