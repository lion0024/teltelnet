#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define PORTNUM		15000												/* リモートlsサーバのポート				*/
#define HOSTLEN		256
#define oops(msg)	{ perror(msg); exit(1); }

int main(int ac, char *av[])
{
	struct  sockaddr_in   saddr;                /* ここにアドレスを構築する       */
	struct  hostent       *hp;                  /* アドレスの一部                 */
	char		hostname[HOSTLEN];									/* 回線ID、ファイルデスクリプタ		*/
	int     sock_id, sock_fd;                   /* 入出力のためのストリーム       */
	FILE		*sock_fpi, *sock_fpo;								/* popenを使ってlsを実行する			*/
	FILE		*pipe_fp;														/* クライアントから送られてくる		*/
	char		dirname[BUFSIZ];										/* ディレクトリ名									*/
	char		command[BUFSIZ];										/* popen用												*/
	int			dirlen, c;													

	/* 
	 * ステップ1：カーネルにソケットを要求する
	 */
 
	sock_id = socket(PF_INET, SOCK_STREAM, 0);  /* ソケットを取得する           */
	if ( sock_id == -1)
		oops("socket");

	/*
	 * ステップ2：ソケットにアドレスをバインドする
	 * 						アドレスはホスト、ポート
	 */

	bzero((void *)&saddr, sizeof(saddr));     /* 構造体をクリアする             */
	gethostname(hostname, HOSTLEN);           /* 私はどこ?                      */
	hp = gethostbyname(hostname);             /* ホスト情報を取得する           */
  
	                                          /* ホスト情報を設定               */
	bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM);          /* ソケットポートを設定           */
	saddr.sin_family = AF_INET;               /* アドレスファミリを設定         */
	
	if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
		oops("bind");

	/*
	 * ステップ3：キューサイズを1としてソケットを着信できる状態にする
	 */

	if (listen(sock_id, 1) != 0)
		oops("listen");

	/*
	 * メインループ：accept(),write(),close()
	 */

	while (1){
		sock_fd = accept(sock_id, NULL, NULL);  /* 着信を待つ                     */
		printf("Wow! got a call!\n");
		if (sock_fd == -1)
			oops("accept");


		/* 読み出し方向をストリームとしてオープン	*/
		if ((sock_fpi = fdopen(sock_fd, "r")) == NULL)
			oops("fdopen reading");
		
		if (fgets(dirname, BUFSIZ-5, sock_fpi) == NULL)
			oops("reading dirname");
		
		/* 書き込み方向をストリームとしてオープン	*/
		if ((sock_fpo = fdopen(sock_fd, "w")) == NULL)
			oops("popen");

		sprintf(command, "%s\0", dirname);
		if ((pipe_fp = popen(command, "r")) == NULL)
			oops("popen");
			
		/* lsからソケットにデータを転送	*/
		while ((c = getc(pipe_fp)) != EOF)
			putc(c, sock_fpo);	
		pclose(pipe_fp);
		fclose(sock_fpo);
		fclose(sock_fpi);
	}
}
