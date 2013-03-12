/* CMS Trouble Notify for KDE4
 * author: Bartosz Świątek (shadzik@gmail.com)
 * license: GPLv2+
 * (c) 2010
 *
 */

#include <KTitleWidget>
#include <KLocale>
#include <KIcon>

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMapIterator>
#include <QDebug>

#include "ktroubledialog.h"

extern "C" {
  #include "ktroublelogin.h"
}


kTroubleDialog::kTroubleDialog(QWidget *parent) : QDialog(parent)
{
}

kTroubleDialog::~kTroubleDialog()
{
}

QWidget* kTroubleDialog::credDialog()
{
setFixedSize(250,200);

wallet = Wallet::openWallet(Wallet::LocalWallet(), winId(), Wallet::Asynchronous);

saveButton = new QPushButton;
saveButton->setText("Save");
saveButton->setIcon(KIcon("dialog-password"));
saveButton->setDisabled(true);

cancelButton = new QPushButton;
cancelButton->setText("Cancel");
cancelButton->setIcon(KIcon("dialog-cancel"));

connect(cancelButton, SIGNAL(clicked()), SLOT(close()));
connect(saveButton, SIGNAL(clicked()), SLOT(doSave()));
connect(wallet, SIGNAL(walletOpened(bool)), SLOT(walletOpened(bool)));

QLabel *titleWidget = new QLabel;
titleWidget->setText("<html><font size=\"5\">"
"Enter your CMS login and password.<br/>"
"<font size=\"3\">"
"They will be securely stored in your kwallet."
"</font></html>");
titleWidget->setWordWrap(true);
titleWidget->setAlignment(Qt::AlignTop);

QLabel *image = new QLabel;
image->setPixmap(KIcon("preferences-desktop-user-password").pixmap(64));
image->setAlignment(Qt::AlignLeft);

loginField = new QLineEdit;
loginField->setEchoMode(QLineEdit::Normal);
loginField->setPlaceholderText("Login");

passwordField = new QLineEdit;
passwordField->setEchoMode(QLineEdit::Password);
passwordField->setPlaceholderText("Password");

QHBoxLayout *imageBox = new QHBoxLayout;
imageBox->addWidget(image);
imageBox->addWidget(titleWidget);

QHBoxLayout *buttonBox = new QHBoxLayout;
buttonBox->addWidget(saveButton);
buttonBox->addWidget(cancelButton);

QVBoxLayout *mainLayout = new QVBoxLayout;
mainLayout->addLayout(imageBox);
mainLayout->addWidget(loginField);
mainLayout->addWidget(passwordField);
mainLayout->addLayout(buttonBox);
mainLayout->setMargin(10);

//QWidget *mainWidget = new QWidget;
//mainWidget->setLayout(mainLayout);
//mainWidget->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

setLayout(mainLayout);

this->exec(); 
}

void kTroubleDialog::walletOpened(bool ok)
{
  if (ok && (wallet->hasFolder(KWallet::Wallet::FormDataFolder()) ||
    wallet->createFolder(KWallet::Wallet::FormDataFolder())) &&
    wallet->setFolder(KWallet::Wallet::FormDataFolder()))
  {
    saveButton->setDisabled(false);
    qDebug() << "Kwallet opened";
  } else
    qDebug() << "Kwallet still closed";
}

void kTroubleDialog::doSave()
{
  if(loginField->text().isEmpty() || passwordField->text().isEmpty())
  {
    QMessageBox::critical(this, "Empty field",
      "You need to fill both fields!",
      QMessageBox::Ok);
    return;
  }
  
  saveButton->setDisabled(true);
  
  QMap<QString, QString> map;
  map[loginField->text()] = passwordField->text();
  
  if(wallet->writeMap("kTroubleNotify", map))
  {
    QMessageBox::critical(this, "Error!",
      "An error occured while saving your credentials!",
      QMessageBox::Ok);
    return;
  } else {
    passwordField->clear();
    loginField->clear();
    close();
  }
}

void kTroubleDialog::close()
{
  this->deleteLater();
}

void kTroubleDialog::checkIfSaved()
{
  wallet = Wallet::openWallet(Wallet::LocalWallet(), 0, Wallet::Synchronous);
  
  // add this once we've started
  if(!ticketsMap.contains("<a href=\"https://trouble.cms.hu-berlin.de\"></a>"))
    ticketsMap["<a href=\"https://trouble.cms.hu-berlin.de\"></a>"] = true;
  
  char **tickets = {NULL};
  
  QMap<QString, QString> value;
  
  QApplication::processEvents(QEventLoop::AllEvents);
  
  if(wallet->hasFolder(KWallet::Wallet::FormDataFolder()))
  {
    wallet->setFolder(KWallet::Wallet::FormDataFolder());
    if(wallet->hasEntry("kTroubleNotify"))
    {
      wallet->readMap("kTroubleNotify", value);
      QMapIterator<QString, QString> i(value);
      while (i.hasNext()) {
	i.next();
	//qDebug() << i.key() << i.value();
	username = i.key();
	password = i.value();
      }
      login(username.toLatin1().data(), password.toLatin1().data());
      //if(!login(username.toLatin1().data(), password.toLatin1().data()))
	//aps->sendNotify("Login failed");
	//qDebug() << "Login failed";
      //else
      //{
	tickets = showTickets();
	//qDebug() << "Sizeof tickets" << sizeof(tickets);
	for (int i=0; i<255; i++)
	{
	  //qDebug() << i;
	  if(tickets[i] != NULL)
	  {
	    //qDebug() << "Ticket 7" << tickets[7];
	    QString link = QString(tickets[i]).toLatin1().data();
	    qDebug() << link;
	    if (QString::compare(link, "none"))
	    {
	      if(!ticketsMap.contains(link))
	      {
		aps = new TroubleNotify();
		aps->sendNotify(link);
		aps->deleteLater();
		ticketsMap[link] = true;
		qDebug() << "Value:" << ticketsMap.value(link);
	      } else {
		qDebug() << "Already got " << link << "in QMap";
	      }
	    }
	    else {
	      qDebug() << "Link was NONE";
	    }
	  }
	  delete tickets[i];
	}
      //}
    } 
    else
    {
      qDebug() << "Wallet has no entry";
      kTroubleDialog *newDiag = new kTroubleDialog;
      newDiag->credDialog();
    }
  } else
    qDebug() << "Wallet is closed";
}
