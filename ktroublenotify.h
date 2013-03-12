/* CMS Trouble Notify for KDE4
 * author: Bartosz Świątek (shadzik@gmail.com)
 * license: GPLv2+
 * (c) 2010
 *
 */

#include <QObject>
#include <QString>

#include <KWallet/Wallet>
#include <KNotification>

#ifndef KTROUBLENOTIFY_H
#define KTROUBLENOTIFY_H

using KWallet::Wallet;

class TroubleNotify: public QObject
{
  Q_OBJECT

  private slots:
    void handleActionClosed();
    
  private:
    void getCredentials();
    KNotification *notify;

  public:
    void sendNotify(QString message);

};

#endif
