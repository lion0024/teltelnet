#include "net_service.h"

#define oops(msg)	{ perror(msg); exit(1); }
#define PORTNUM		15000
#define MAXARGS   20
#define ARGLEN    100

int main(int ac, char *av[])
{
	char  *arglist[MAXARGS+1];                    /* ポインタの配列               */ 
	int   numargs;                                /* 配列の添字                   */
	char  argbuf[ARGLEN];                         /* ここに入力を読み込む         */
	struct	sockaddr_in		servadd;								/* 呼び出す番号									*/
	struct	hostent				*hp;										/* 番号の取得										*/
	char  *makestring();                          /* メモリの確保などを実行       */
	int			sock_id, sock_fd;											/* ソケットとfd									*/
	char		buffer[BUFSIZ];												/* 受信メッセージ								*/
	int			n_read;																/* メッセージサイズ							*/

	/*if (ac != 3) exit(1);*/

	/* 
	 * ステップ1：ソケットを作成する
	 */

	sock_id = socket(AF_INET, SOCK_STREAM, 0);		/* 回線を取得する								*/
	if ( sock_id == -1){
		oops("socket");															/* 失敗													*/
	}

	/* 
	 * ステップ2：サーバに接続する
	 */

	bzero(&servadd, sizeof(servadd));							/* アドレスを0クリアする				*/
	hp = gethostbyname(av[1]);										/* ホストのIPアドレスを照合する	*/
	if (hp == NULL){
		oops(av[1]);																/* 失敗													*/
	}
	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(PORTNUM);						/* ポート番号を設定							*/
	servadd.sin_family = AF_INET;									/* ソケットタイプを設定する			*/

	if (connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0){
		oops("connect");
	}

	/*
	 * ステップ3：ディレクトリ名を送り、結果を読み出す
	 */

	numargs = 0;
	while (numargs < MAXARGS)
	{
		printf("Arg[%d]? ", numargs);
		if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n'){
			arglist[numargs++] = makestring(argbuf);
		} else {
			if(numargs > 0){
				arglist[numargs] = NULL;
				if (write(sock_id, arglist[0], strlen(arglist[0])) == -1){
					oops("write");
				}
				if (write(sock_id, "\n", 1) == -1){
					oops("write");
				}
				while ((n_read = read(sock_id, buffer, BUFSIZ)) > 0){
					if (write(1, buffer, n_read) == -1){
						oops("write");
					}
				}
				close(sock_id);
			}
		}
	}
	return 0;
}


char *makestring(char *buf)
/*
 * 改行を取り除き、文字列の記憶領域を作る
 */
{
	char  *cp;

	buf[strlen(buf)-1] = '\0';            /* 改行を削除         */
	cp = malloc(strlen(buf)+1);           /* メモリを確保       */
	if (cp == NULL){                      /* 失敗したら終了     */
		fprintf(stderr, "no memory\n");
		exit(1);
	}
	strcpy(cp, buf);                      /* 文字をコピーする   */
	return cp;                            /* ポインタを返す     */
}
    
