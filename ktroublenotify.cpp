/* CMS Trouble Notify for KDE4
 * author: Bartosz Świątek (swiateba@cms.hu-berlin.de)
 * license: GPLv2+
 * (c) 2010
 *
 */

#include <QApplication>
#include <QObject>

#include <KDebug>
#include <KLocale>
#include <kworkspace/kworkspace.h>

#include "ktroublenotify.h"

void TroubleNotify::handleActionClosed()
{
	kDebug() << "Close";
	notify->close();
	notify->deleteLater();
}

void TroubleNotify::sendNotify(QString message)
{
	notify = new KNotification("NewTicketArrived", 0, KNotification::Persistent);
	notify->setTitle("New ticket arrived");

	QString text;
	text += "<html>" + message + "<br/></html>";
	notify->setText(text);

	QStringList actions;
	actions << i18n("Close");
	notify->setActions(actions);

	connect(notify, SIGNAL(activated(uint)), this, SLOT(handleActionClosed()));

	notify->sendEvent();
}

