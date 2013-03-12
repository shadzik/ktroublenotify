/* CMS Trouble Notify for KDE4
 * author: Bartosz Świątek (swiateba@cms.hu-berlin.de)
 * license: GPLv2+
 * (c) 2010
 *
 */

#include <stdio.h>
#include <tidy.h>
#include <buffio.h>
#include <curl/curl.h>

uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out);
void dumpNode(TidyDoc doc, TidyNode tnod, int indent);
void getTickets(TidyDoc doc, TidyNode tnod, int indent, int i);
int login(char *login, char *pass);
char** showTickets();
