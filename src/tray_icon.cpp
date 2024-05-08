#include "tray_icon.h"

#include <QAction>
#include <QCoreApplication>
#include <QMainWindow>
#include <QMenu>

TrayIcon::TrayIcon(QObject *parent)
    : QObject{parent}
{
    createAction();
    m_disableProxyAction->setEnabled(false);

    createTrayIcon();
    connect(m_trayIcon, &QSystemTrayIcon::activated, this,
            &TrayIcon::iconActivated);
}

TrayIcon::~TrayIcon()
{
    delete m_trayIconMenu;
}

void TrayIcon::show()
{
    m_trayIcon->show();
}

void TrayIcon::setIcon(const QIcon &icon)
{
    m_trayIcon->setIcon(icon);
}

bool TrayIcon::isVisible() const
{
    return m_trayIcon->isVisible();
}

void TrayIcon::setMenuEnabled(bool proxy_enabled)
{
    m_enableProxyAction->setEnabled(!proxy_enabled);
    m_disableProxyAction->setEnabled(proxy_enabled);
}

void TrayIcon::createAction()
{
    m_restoreAction = new QAction(tr("&Restore"), this);
    connect(m_restoreAction, &QAction::triggered, this,
            &TrayIcon::restoreActionTriggered);

    m_quitAction = new QAction(tr("&Quit"), this);
    connect(m_quitAction, &QAction::triggered, qApp,
            &QCoreApplication::quit);

    m_enableProxyAction = new QAction(tr("Enble proxy"), this);
    connect(m_enableProxyAction, &QAction::triggered, this,
            &TrayIcon::enableProxyActionTriggered);

    m_disableProxyAction = new QAction(tr("Disable proxy"), this);
    connect(m_disableProxyAction, &QAction::triggered, this,
            &TrayIcon::disableProxyActionTriggered);
}

void TrayIcon::createTrayIcon()
{
    m_trayIconMenu = new QMenu();
    m_trayIconMenu->addAction(m_restoreAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_enableProxyAction);
    m_trayIconMenu->addAction(m_disableProxyAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_quitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    QIcon icon(":/images/app.ico");
    m_trayIcon->setIcon(icon);
    m_trayIcon->setToolTip("qsing-box");
    m_trayIcon->setContextMenu(m_trayIconMenu);
}
