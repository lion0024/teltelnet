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
	
	#define PORTNUM   15000                       /* リモートサーバのポート       */
	#define oops(msg) { perror(msg); exit(1); }  
#endif
