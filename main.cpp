/* CMS Trouble Notify for KDE4
 * author: Bartosz Świątek (shadzik@gmail.com)
 * license: GPLv2+
 * (c) 2010
 *
 */

#include <QObject>
#include <QApplication>

#include <KIcon>

#include "ktroubledialog.h"
#include "ktroubletray.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  
  ktroubletray *appTray = new ktroubletray;
  appTray->startChecking();
  
  return app.exec();
}

