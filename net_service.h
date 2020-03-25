/* net_service.h - ヘッダーファイル
 */

#ifndef _net_service_h_
	#include <stdio.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <string.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <time.h>
	#include <errno.h>
	
	#define PORTNUM   15000                       /* リモートサーバのポート       */
	#define oops(msg, errcode, status)\
		{ fprintf(stderr, "[ERROR] %s errcode:%d status:%d\n",\
		 msg, errcode, status); exit(1); }
#endif
