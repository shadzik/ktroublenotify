/* CMS Trouble Notify for KDE4
 * author: Bartosz Świątek (shadzik@gmail.com)
 * license: GPLv2+
 * (c) 2010
 *
 */

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QDialog>
#include <QApplication>
#include <QString>
#include <QMap>
#include <QList>

#include <KWallet/Wallet>

#include "ktroublenotify.h"

#ifndef KTROUBLEDIALOG_H
#define KTROUBLEDIALOG_H

using KWallet::Wallet;

class kTroubleDialog : public QDialog
{
  
  Q_OBJECT
  
public:
  kTroubleDialog(QWidget *parent = 0);
  virtual ~kTroubleDialog();
  QWidget * credDialog();
  void checkIfSaved();
  QString username, password;
  
private Q_SLOTS:
  void close();
  void doSave();
  void walletOpened(bool ok);
  
private:
  QPushButton *saveButton, *cancelButton;
  QLineEdit *loginField, *passwordField;
  Wallet *wallet;
  QMap<QString, bool> ticketsMap;
  TroubleNotify *aps;
  
};

#endif // KTROUBLEDIALOG_H
