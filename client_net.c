/*****************************************************************
ファイル名	: client_net.c
機能		: クライアントのネットワーク処理
*****************************************************************/

#include"common.h"
#include"client_func.h"
#include<sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include<netdb.h>

#include <unistd.h> /* for close */
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#define	BUF_SIZE	1024


static int	gSocket;	/* ソケット */
static fd_set	gMask;	/* select()用のマスク */
static int	gWidth;		/* gMask中ののチェックすべきビット数 */

static int sendsock,recvsock;
static struct sockaddr_in recv_addr, send_addr;
static char buf[2048];
CLIENT gClients[MAX_CLIENTS];

/*****************************************************************
関数名	: SetUpClient
機能	: サーバーとのコネクションを設立し，
		  ユーザーの名前の送受信を行う
引数	: char	*hostName		: ホスト
		  int	*num			: 全クライアント数
		  char	clientNames[][]		: 全クライアントのユーザー名
出力	: コネクションに失敗した時-1,成功した時0
*****************************************************************/
int SetUpClient(char *hostName,int *clientID,char clientNames[][MAX_NAME_SIZE])
{

struct hostent *servHost;
int	len;
char	str[100];
int i;
char *ipad;

/* ホスト名からホスト情報を得る */
if((servHost = gethostbyname(hostName))==NULL){
fprintf(stderr,"Unknown host\n");
return -1;
}

 char hostname[128];
 char sendData[1024];
  /* ホスト名を取得 */
  gethostname(hostname, sizeof(hostname));
 
  /* 取得したホスト名を出力 */
  printf("ホスト名: %s\n", hostname);

    sendsock = socket(AF_INET, SOCK_DGRAM, 0);
    recvsock = socket(AF_INET, SOCK_DGRAM, 0);
    
    send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons(20000);
    //send_addr.sin_addr.s_addr =hostName;

 bcopy(servHost->h_addr,(char*)&send_addr.sin_addr,servHost->h_length);
        
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(30000);
    recv_addr.sin_addr.s_addr = INADDR_ANY;

    bind(recvsock, (struct sockaddr *)&recv_addr, sizeof(recv_addr));

    fprintf(stderr,"connected\n");

    //sprintf(sendData,"1\0");


    printf("名前を入力してください\n");
    scanf("%s",str);
  
    sprintf(sendData, "%s,%s\0", str, hostname);

    printf("%s\n",sendData);
    SendData(sendData);

    printf("Please Wait\n");

    for(i=0;i<2;i++){
    recv(recvsock, buf, sizeof(buf), 0);

    printf("%s\n",buf);

    char *app_id = strtok(buf, ",");
    char *name = strtok(NULL, ",");
    char *host = strtok(NULL, ",");
    int id=atoi(strtok(NULL, ","));
    strcpy(gClients[id].name,name);

    if(strcmp(hostname,host)==0)
       *clientID=id;

printf("app_id = %s\n",app_id);
printf("name = %s\n",name);
printf("host = %s\n",host);
printf("id   = %d\n",id);

    }

        printf("clientID=%d\n",*clientID);
recv(recvsock, buf, sizeof(buf), 0);
printf("%s\n",buf);

recv(recvsock, buf, sizeof(buf), 0);
cnum=atoi(buf);
printf("clientnum=%d\n",cnum);


    /* 全クライアントのユーザー名を得る */
    //GetAllName(clientID,num,clientNames);

    /* select()のためのマスク値を設定する */
    //SetMask();
    
    return 0;
}

/*****************************************************************
関数名	: SendRecvManager
機能	: サーバーから送られてきたデータを処理する
引数	: なし
出力	: プログラム終了コマンドが送られてきた時0を返す．
		  それ以外は1を返す
*****************************************************************/
int SendRecvManager(void)
{
    recv(recvsock, buf, sizeof(buf), 0);
    printf("%s\n",buf);

    int		endFlag = 1;

    	/* コマンドに対する処理を行う */
		endFlag = ExecuteCommand(buf);
                printf("%d\n",endFlag);
    // }
    return endFlag;
}

/*****************************************************************
関数名	: SendData
機能	: サーバーにデータを送る
引数	: void		*data		: 送るデータ
		  int		dataSize	: 送るデータのサイズ
出力	: なし
*****************************************************************/
void SendData(char *data)
{
    int length = 0;
    while(1) {
        if (data[length] == '\0'){
            length++;
            break;
        }
        length++;
    }

    sendto(sendsock,data,length,0,(struct sockaddr *)&send_addr, sizeof(send_addr));

}

/*****************************************************************
関数名	: CloseSoc
機能	: サーバーとのコネクションを切断する
引数	: なし
出力	: なし
*****************************************************************/
void CloseSoc(void)
{
    printf("...Connection closed\n");
    close(gSocket);
}

