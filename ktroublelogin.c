/* CMS Trouble Notify for KDE4
 * author: Bartosz Świątek (shadzik@gmail.com)
 * license: GPLv2+
 * (c) 2010
 *
 */

#include "ktroublelogin.h"

char *ltval;
char *ticketTitle[256] = {NULL}, *ticketHref[256] = {NULL};

uint write_cb(char* in, uint size, uint nmemb, TidyBuffer* out)
{
  uint r;
  r = size * nmemb;
  tidyBufAppend( out, in, r );
  return(r);
}

void dumpNode(TidyDoc doc, TidyNode tnod, int indent)
{
  TidyNode child;
  for ( child = tidyGetChild(tnod); child; child = tidyGetNext(child) )
  {
    if ( tidyNodeIsINPUT )
    {
      TidyAttr attr, val;
      attr = tidyAttrGetNAME(child);
      val = tidyAttrGetVALUE(child);
      if(tidyAttrValue(attr))
      {
        if(!strcmp(tidyAttrValue(attr),"lt")) {
          ltval = (char *)tidyAttrValue(val);
        }
      }
    }
    dumpNode( doc, child, indent + 4 ); /* recursive */
  }
}

void getTickets(TidyDoc doc, TidyNode tnod, int indent, int i )
{
  TidyNode child;
  for ( child = tidyGetChild(tnod); child; child = tidyGetNext(child) )
  {
    if ( tidyNodeIsLINK )
    {
      TidyAttr title, href;
      title = tidyAttrGetTITLE(child);
      href = tidyAttrGetHREF(child);
      if(tidyAttrValue(title))
      {
         if(tidyAttrValue(href)!=NULL)
	 {
	   ticketTitle[i] = tidyAttrValue(title);
	   ticketHref[i] = tidyAttrValue(href);
	   /*
           printf("<a href=\"https://trouble.cms.hu-berlin.de%s\">%s</a>\n", ticketHref[i], ticketTitle[i]);
	   
	   printf("Iterator=%d tickets=%d\n", i, sizeof(ticketTitle)/sizeof(char*));
	   */
	 }
      }
    }
    getTickets( doc, child, indent + 4, ++i); /* recursive */
  }
}

int login(char *login, char *pass)
{
  char * username = "username=";
  char * password = "&password=";
  char * eventid = "&_eventId=submit&";
  char * lt = "lt=";
  CURL *curl;
  char curl_errbuf[CURL_ERROR_SIZE];
  TidyDoc tdoc;
  TidyBuffer docbuf = {0};
  TidyBuffer tidy_errbuf = {0};
  int err;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "https://cas.hu-berlin.de/cas/login");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/cascookie");
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "/tmp/cascookie");

    tdoc = tidyCreate();
    tidyOptSetBool(tdoc, TidyForceOutput, yes); /* try harder */
    tidyOptSetInt(tdoc, TidyWrapLen, 4096);
    tidySetErrorBuffer( tdoc, &tidy_errbuf );
    tidyBufInit(&docbuf);
    
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);
    err=curl_easy_perform(curl);
    if ( !err ) {
      err = tidyParseBuffer(tdoc, &docbuf); /* parse the input */
      if ( err >= 0 ) {
        err = tidyCleanAndRepair(tdoc); /* fix any problems */
        if ( err >= 0 ) {
          err = tidyRunDiagnostics(tdoc); /* load tidy error buffer */
          if ( err >= 0 ) {
            dumpNode( tdoc, tidyGetRoot(tdoc), 0 );
            if(ltval!=NULL) {
              char *data = malloc(strlen(username) + strlen(login) + strlen(password) + strlen(pass)
                           + strlen(eventid) + strlen(lt) + strlen(ltval) + 8);
              strcpy(data, username);
              strcat(data, login);
              strcat(data, password);
              strcat(data, pass);
              strcat(data, eventid);
              strcat(data, lt);
              strcat(data, ltval);
              curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
              curl_easy_perform(curl);
              curl_easy_cleanup(curl);
	    }
          }
        }
      }
    }
    else
      fprintf(stderr, "%s\n", curl_errbuf);

    /* clean-up */
    /* curl_easy_cleanup(curl); */
    tidyBufFree(&docbuf);
    tidyBufFree(&tidy_errbuf);
    tidyRelease(tdoc);
    if(ltval!=NULL)
      return 0;
    else
      return 1;
}

char ** showTickets()
{
  int i = 0;
  for (i=0; i<sizeof(ticketTitle)/sizeof(char*); i++)
  {
    ticketTitle[i] = NULL;
    ticketHref[i] = NULL;
  }
  char * beginHref = "<a href=\"https://trouble.cms.hu-berlin.de";
  char * endBeginHref = "\">";
  char * endHref = "</a>";
  char * result[4096] = {NULL};
  CURL *curl;
  char curl_errbuf[CURL_ERROR_SIZE];
  TidyDoc tdoc;
  TidyBuffer docbuf = {0};
  TidyBuffer tidy_errbuf = {0};
  int err;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "https://trouble.cms.hu-berlin.de/otrs/index.pl?Action=AgentDashboard&Subaction=Element&Name=0120-TicketNew&Filter=New&StartWindow=0&StartHit=1");
    /*
    curl_easy_setopt(curl, CURLOPT_URL, "https://trouble.cms.hu-berlin.de/otrs/index.pl?Action=AgentDashboard&Subaction=Element&Name=0130-TicketOpen&Filter=New&StartWindow=0&StartHit=1");
    */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/cascookie");

    tdoc = tidyCreate();
    tidyOptSetBool(tdoc, TidyForceOutput, yes); /* try harder */
    tidyOptSetInt(tdoc, TidyWrapLen, 4096);
    tidySetErrorBuffer( tdoc, &tidy_errbuf );
    tidyBufInit(&docbuf);
    
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);
    err=curl_easy_perform(curl);
    if ( !err ) {
      err = tidyParseBuffer(tdoc, &docbuf); /* parse the input */
      if ( err >= 0 ) {
        err = tidyCleanAndRepair(tdoc); /* fix any problems */
        if ( err >= 0 ) {
          err = tidyRunDiagnostics(tdoc); /* load tidy error buffer */
	  getTickets( tdoc, tidyGetRoot(tdoc), 0, 0 );
          if ( err >= 0 ) {
	    for(i=0; i<sizeof(ticketTitle)/sizeof(char*); i++)
	    {
	      if((ticketHref[i] != NULL) && (ticketTitle[i] != NULL))
	      {
		result[i] = malloc(strlen(beginHref) + strlen(ticketHref[i]) + strlen(endBeginHref) 
			+ strlen(ticketTitle[i]) + strlen(endHref) + 5);
		strcpy(result[i], beginHref);
		strcat(result[i], ticketHref[i]);
		strcat(result[i], endBeginHref);
		strcat(result[i], ticketTitle[i]);
		strcat(result[i], endHref);
		printf("Iterator: %d Result: %s\n", i, result[i]);
	      }
	    }
          }
        }
      }
    }
    else
      fprintf(stderr, "%s\n", curl_errbuf);

    /* clean-up */
    curl_easy_cleanup(curl);
    tidyBufFree(&docbuf);
    tidyBufFree(&tidy_errbuf);
    tidyRelease(tdoc);
    if(result!=NULL)
      return result;
    else
      return "none";

}
