/* CMS Trouble Notify for KDE4
 * author: Bartosz Świątek (shadzik@gmail.com)
 * license: GPLv2+
 * (c) 2010
 *
 */

#include <QObject>

#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>

#include <KWallet/Wallet>

#ifndef KTROUBLETRAY_H
#define KTROUBLETRAY_H

#include "ktroubledialog.h"

using KWallet::Wallet;

class ktroubletray: public QObject
{
  Q_OBJECT
  
private Q_SLOTS:
  void close();
  void addUser();
  void clearData();
  void update();
  void clearCookie();
  
public:
  ktroubletray();
  virtual ~ktroubletray();
  void startChecking();
  
private:
  QMenu *trayMenu;
  QSystemTrayIcon *tray;
  Wallet *wallet;
  QAction *quit, *addCred, *clear, *refresh, *remCookie;
  kTroubleDialog *diag;
  
};

#endif // KTROUBLETRAY_H
