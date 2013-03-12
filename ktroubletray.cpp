/* CMS Trouble Notify for KDE4
 * author: Bartosz Świątek (swiateba@cms.hu-berlin.de)
 * license: GPLv2+
 * (c) 2010
 *
 */

#include <KIcon>
#include <QDebug>
#include <QApplication>
#include <QTimer>
#include <QFile>
#include <QDir>

#include "ktroubletray.h"

ktroubletray::ktroubletray() : QObject()
{
  diag = new kTroubleDialog;
  
  refresh = new QAction(this);
  refresh->setIcon(KIcon("task-recurring"));
  refresh->setText("Refresh now");
  
  clear = new QAction(this);
  clear->setIcon(KIcon("list-remove-user"));
  clear->setText("Clear credentials");
  
  addCred = new QAction(this);
  addCred->setIcon(KIcon("list-add-user"));
  addCred->setText("Add credentials");
  addCred->setDisabled(true);
  
  remCookie = new QAction(this);
  remCookie->setIcon(KIcon("edit-delete"));
  remCookie->setText("Delete cookie");
  
  quit = new QAction(this);
  quit->setIcon(KIcon("application-exit"));
  quit->setText("Quit");
  
  trayMenu = new QMenu;
  trayMenu->setTitle("CMS K Trouble Notify");
  trayMenu->addAction(refresh);
  trayMenu->addAction(addCred);
  trayMenu->addAction(clear);
  trayMenu->addSeparator();
  trayMenu->addAction(remCookie);
  trayMenu->addSeparator();
  trayMenu->addAction(quit);
  
  connect(quit, SIGNAL(triggered(bool)), this, SLOT(close()));
  connect(addCred, SIGNAL(triggered(bool)), this, SLOT(addUser()));
  connect(clear, SIGNAL(triggered(bool)), this, SLOT(clearData()));
  connect(refresh, SIGNAL(triggered(bool)), this, SLOT(update()));
  connect(remCookie, SIGNAL(triggered(bool)), this, SLOT(clearCookie()));
  
  tray = new QSystemTrayIcon;
  tray->setIcon(KIcon("dialog-password"));
  tray->setVisible(true);
  tray->setToolTip("CMS K Trouble Notify");
  tray->setContextMenu(trayMenu);
  tray->show();

}

ktroubletray::~ktroubletray()
{

}

void ktroubletray::close()
{
  QApplication::quit();
}

void ktroubletray::addUser()
{
  kTroubleDialog *newDiag = new kTroubleDialog;
  newDiag->credDialog();
  if(newDiag->isWindow())
  {
    addCred->setDisabled(true);
    clear->setDisabled(false);
  }
}

void ktroubletray::clearData()
{
  wallet = Wallet::openWallet(Wallet::LocalWallet(), 0, Wallet::Synchronous);
  
  if(wallet->hasFolder(KWallet::Wallet::FormDataFolder()))
  {
    wallet->setFolder(KWallet::Wallet::FormDataFolder());
    if(wallet->hasEntry("kTroubleNotify"))
    {
      wallet->removeEntry("kTroubleNotify");
      addCred->setDisabled(false);
      clear->setDisabled(true);
    }
  }
}

void ktroubletray::startChecking()
{
  QApplication::processEvents(QEventLoop::AllEvents);
  //QTimer::singleShot(100, this, SLOT(clearCookie()));
  QTimer::singleShot(200, this, SLOT(update()));
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(3*60000); //3 mins
  //QTimer *ccTimer = new QTimer(this);
  //connect(ccTimer, SIGNAL(timeout()), this, SLOT(clearCookie()));
  //ccTimer->start(8*60*60000); // 8 hours
}

void ktroubletray::update()
{
  qDebug() << "Checking for Tickets";
  diag->checkIfSaved();
}

void ktroubletray::clearCookie()
{
  QFile *cookie = new QFile("/tmp/cascookie");
  qDebug() << "Removing cookie" << cookie->fileName();
  if(cookie->exists())
  {
    cookie->remove();
    qDebug() << "Removed";
  }
  cookie->deleteLater();
}