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
		  int	*client			: クライアントID
		  char	clientNames[][]		: 全クライアントのユーザー名
出力	: コネクションに失敗した時-1,成功した時0
*****************************************************************/
int SetUpClient(char *hostName,int *clientID,char clientNames[][MAX_NAME_SIZE])
{
    
    struct hostent *servHost;
    int	len;
    char	str[16];//name入力
    int     namelen;//(/n)以外の長さ
    int     err;//エラー
    int i=0;
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
    
    /*受信用と送信用ソケットの作成*/
    sendsock = socket(AF_INET, SOCK_DGRAM, 0);
    recvsock = socket(AF_INET, SOCK_DGRAM, 0);
    
    send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons(20000);
    
    bcopy(servHost->h_addr,(char*)&send_addr.sin_addr,servHost->h_length);
    
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(30000);
    recv_addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(recvsock, (struct sockaddr *)&recv_addr, sizeof(recv_addr));
    

    fprintf(stderr,"connected\n");

    /*名前の入力*/
    do{
        err = 0;//指定文字数以内で入力されるまで繰り返す
        printf("名前を入力してください\n");
        fgets(str, sizeof(str), stdin);//文字列の読み込み
        *strchr(str,'\n') = '\0';//含まれた改行を消す
        namelen = strlen(str);
        
        if(namelen == 0){//なにも入力されてないときのエラーメッセージ
            printf("1文字以上で");
            err=1;
        }
        else if(namelen > 8){//指定文字数以上のときのエラーメッセージ
            printf("8文字以内で");
            err = 1;
        }
    }while(err);
    
    InitWindows();
    
    /*通信ができる状態であることをサーバーに伝えるため「1」を送る*/
    sprintf(sendData,"1\0");
    SendData(sendData);
    printf("sendData = %s\n",sendData);

    printf("Please Wait\n");

    /*クライアントが全員送ったらサーバーから「1」が送られるのでループを抜ける*/
    while(i==0){
        recv(recvsock, buf, sizeof(buf), 0);

        i=atoi(buf);
    }
    
    /*サーバーからクライアントの数が送られる*/
    recv(recvsock, buf, sizeof(buf), 0);
    cnum=atoi(buf);

    /*サーバーに名前とホスト名を送る*/
    sprintf(sendData, "%s,%s\0", str, hostname);
    SendData(sendData);
    
    printf("Please Wait\n");
    

    /*サーバーからそれぞれのクライアント名前、ホスト名、IDが送られてくる*/
    for(i=0;i<cnum;i++){
        recv(recvsock, buf, sizeof(buf), 0);
        
        printf("%s\n",buf);
        
        char *app_id = strtok(buf, ",");
        char *name = strtok(NULL, ",");
        char *host = strtok(NULL, ",");
        int id=atoi(strtok(NULL, ","));
        strcpy(gClients[id].name,name);
        
        /*ホスト名が自分と同じであればそのクライアントは自分なので、自身のIDを保管する*/
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
    
    int		endFlag = 1;
    
    /* コマンドに対する処理を行う */
    endFlag = ExecuteCommand(buf);

    return endFlag;
}

/*****************************************************************
関数名	: SendData
機能	: サーバーにデータを送る
引数	: void		*data		: 送るデータ
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

