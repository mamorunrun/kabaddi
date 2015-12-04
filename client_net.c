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

static void GetAllName(int *clientID,int *num,char clientNames[][MAX_NAME_SIZE]);
static void SetMask(void);
static int RecvData(void *data,int dataSize);

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
int SetUpClient(char *hostName,int *clientID,int *num,char clientNames[][MAX_NAME_SIZE])
{

struct hostent *servHost;
int	len;
char	str[100];
int i,cnum;
char *ipad;

/* ホスト名からホスト情報を得る */
if((servHost = gethostbyname(hostName))==NULL){
fprintf(stderr,"Unknown host\n");
return -1;
}

 char hostname[128];
 
  /* ホスト名を取得 */
  gethostname(hostname, sizeof(hostname));
 
  /* 取得したホスト名を出力 */
  printf("ホスト名: %s\n", hostname);

 int fd;
 struct ifreq ifr;

 fd = socket(AF_INET, SOCK_DGRAM, 0);

 /* IPv4のIPアドレスを取得したい */
 ifr.ifr_addr.sa_family = AF_INET;

 /* eth0のIPアドレスを取得したい */
 strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

 ioctl(fd, SIOCGIFADDR, &ifr);

 close(fd);

 /* 結果を表示 */
 printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
 ipad=inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
 printf("%s\n",ipad);

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

    scanf("%s",str);

    char sendData[1024];
  
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

    /* 全クライアントのユーザー名を得る */
    GetAllName(clientID,num,clientNames);

    /* select()のためのマスク値を設定する */
    SetMask();
    
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
    fd_set	readOK;
    char	command;
    int		i;
    int		endFlag = 1;
    struct timeval	timeout;

    /* select()の待ち時間を設定する */
    timeout.tv_sec = 0;
    timeout.tv_usec = 20;

    readOK = gMask;
    /* サーバーからデータが届いているか調べる */
    select(gWidth,&readOK,NULL,NULL,&timeout);
    if(FD_ISSET(gSocket,&readOK)){
		/* サーバーからデータが届いていた */
    	/* コマンドを読み込む */
		RecvData(&command,sizeof(char));
    	/* コマンドに対する処理を行う */
		endFlag = ExecuteCommand(command);
    }
    return endFlag;
}

/*****************************************************************
関数名	: RecvIntData
機能	: サーバーからint型のデータを受け取る
引数	: int		*intData	: 受信したデータ
出力	: 受け取ったバイト数
*****************************************************************/
int RecvIntData(int *intData)
{
    int n,tmp;
    
    /* 引き数チェック */
    assert(intData!=NULL);

    n = RecvData(&tmp,sizeof(int));
    (*intData) = ntohl(tmp);
    
    return n;
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

/*****
static
*****/
/*****************************************************************
関数名	: GetAllName
機能	: サーバーから全クライアントのユーザー名を受信する
引数	: int		*num			: クライアント数
		  char		clientNames[][]	: 全クライアントのユーザー名
出力	: なし
*****************************************************************/
static void GetAllName(int *clientID,int *num,char clientNames[][MAX_NAME_SIZE])
{
    int	i;

    /* クライアント番号の読み込み */
    RecvIntData(clientID);
    /* クライアント数の読み込み */
    RecvIntData(num);

    /* 全クライアントのユーザー名を読み込む */
    for(i=0;i<(*num);i++){
		RecvData(clientNames[i],MAX_NAME_SIZE);
    }
#ifndef NDEBUG
    printf("#####\n");
    printf("client number = %d\n",(*num));
    for(i=0;i<(*num);i++){
		printf("%d:%s\n",i,clientNames[i]);
    }
#endif
}


/*****************************************************************
関数名	: SetMask
機能	: select()のためのマスク値を設定する
引数	: なし
出力	: なし
*****************************************************************/
static void SetMask(void)
{
    int	i;

    FD_ZERO(&gMask);
    FD_SET(gSocket,&gMask);

    gWidth = gSocket+1;
}

/*****************************************************************
関数名	: RecvData
機能	: サーバーからデータを受け取る
引数	: void		*data		: 受信したデータ
		  int		dataSize	: 受信するデータのサイズ
出力	: 受け取ったバイト数
*****************************************************************/
int RecvData(void *data,int dataSize)
{
    int n;

    /* 引き数チェック */
    assert(data != NULL);
    assert(0 < dataSize);

    return read(gSocket,data,dataSize);
}
