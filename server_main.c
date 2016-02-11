/*****************************************************************
ファイル名	: server_main.c
機能		: サーバーのメインルーチン
*****************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "common.h"

static char buf[2048];
int main(int argc,char *argv[])
{
    int sendsock,recvsock;
    struct sockaddr_in recv_addr, send_addr;
    //char buf[2048];
    int yes = 1;
    int state1=0;
    int state2=0;
    char client_name[MAX_CLIENTS][128];
    int client_num;
    int connection=0;
    int connection_num=0;
    int i;
    int endflag=1;
    char *app_id;
    char *end;

    /*2番目に引き数がクライアントの数*/
    if(argc==2){
        client_num=atoi(argv[1]);
    }
    else exit(-1);

    /*受信用と送信用のソケットを作成*/
    sendsock = socket(AF_INET, SOCK_DGRAM, 0);
    recvsock = socket(AF_INET, SOCK_DGRAM, 0);
    
    send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons(30000);
    send_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
        
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(20000);
    recv_addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(recvsock, (struct sockaddr *)&recv_addr, sizeof(recv_addr));
    
    setsockopt(sendsock,SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof(yes));

    printf("wait\n");

    /*クライアントが全員ソケットを作り、通信ができる状態であることを確認する*/
    while(state1==0){
        recv(recvsock, buf, sizeof(buf), 0);
  
        i=atoi(buf);

        /*クライアントから「1」というデータが送られた時*/
        if(i==1){
            connection++;
        }

        /*クライアントの数だけデータが送られたらそれをクライアントに伝えるため「1」を送りループを抜ける*/
        if(connection==client_num){
            state1=1;
            sprintf(buf,"1\0");
            sendto(sendsock, buf, sizeof(buf), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
        }
        
    }

    /*クライアントの数を全クライアントに伝える*/
    sprintf(buf,"%d\0",client_num);
    sendto(sendsock, buf, sizeof(buf), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));

    /*それぞれクライアントの名前、IDを全クライアントに伝える*/
    while(state2==0) {
        recv(recvsock, buf, sizeof(buf), 0);

        if(state2==0){

            strcpy(client_name[connection_num],buf);
            
            sprintf(buf,"kabaddi,%s,%d\0",client_name[connection_num],connection_num);
            connection_num++;
            sendto(sendsock, buf, sizeof(buf), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));

            if(connection_num==client_num){
                state2=1;
                sprintf(buf,"kabaddi,u%d,",connection_num);
                for(i=0;i<connection_num;i++){
                    strcat(buf,client_name[i]);
                    strcat(buf,",");
                }
                strcat(buf,"\0");
                sendto(sendsock, buf, sizeof(buf), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
            }
            
        }
    }

    /*来たデータを全クライアントに送り続ける*/
    while(endflag){
        recv(recvsock, buf, sizeof(buf), 0);

        printf("%s\n",buf);

        sendto(sendsock, buf, sizeof(buf), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));

        /*データの先頭が「endkabaddi」であればループを抜け終了*/
        if(strncmp(buf,"endkabaddi",10)==0)
            endflag=0;

        }
    
    close(recvsock);
    close(sendsock);

    return 0;
}
