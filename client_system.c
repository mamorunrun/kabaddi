#include"common.h"
#include"client_func.h"


void UpdatePos(int n,int x,int y)
{
    if(clientID == n)
        return;

    gClients[n].poi.x=x;
    gClients[n].poi.y=y;
}

/*********************************************************
自分のキャラの移動と当たり判定をサーバーに送信
                   clientID :自分のID
                   cnum     :クライアントの数
                   befx,befy:前のx座標y座標
当たり判定（壁,キャラ）があればキャラの座標を戻す
*********************************************************/
void Move(int clientID,int befx,int befy)
{
    char	data[MAX_DATA];
    int			dataSize = 0;

    int i = -1;

    dflag = 1;
    
    //printf("Move\n");

    switch(gClients[clientID].ADsta){

    case 0:
        if(gClients[clientID].poi.x <= 0)
            gClients[clientID].poi.x = 0;
        else if(gClients[clientID].poi.x + 30 >= 800)
            gClients[clientID].poi.x = 800 - 30;
        if(gClients[clientID].poi.y <= 0)
            gClients[clientID].poi.y = 0;
        else if(gClients[clientID].poi.y + 30 >= 600)
            gClients[clientID].poi.y = 600 -30;
        break;        

    case 1:
        if(gClients[clientID].poi.x <= 0)
            gClients[clientID].poi.x = 0;
        else if(gClients[clientID].poi.x + 30 >= 1000)
            gClients[clientID].poi.x = 1000 - 30;
        if(gClients[clientID].poi.y <= 0)
            gClients[clientID].poi.y = 0;
        else if(gClients[clientID].poi.y + 30 >= 600)
            gClients[clientID].poi.y = 600 -30;
        break;
        
    }

    i=Collision(clientID,befx,befy);
    
    if(gClients[clientID].Bflag > 0){
        if(gClients[clientID].poi.x >= 600){
            /*WIN,LOSEフラグの送信
              
              sprintf(data,"kabaddi,%d,%d,%d,%d\0",LOSE,clientID,i,0);
              SendData(data);
              
              
              win();
              break;
            */
            
        }
    }    
        printf("%d\n",i);        

        if( i == -1){
            sprintf(data,"kabaddi,%d,%d,%d,%d\0",CDRAW,clientID,gClients[clientID].poi.x,gClients[clientID].poi.y);
    }
        else {
            sprintf(data,"kabaddi,%d,%d,%d,%d\0",BUMP,clientID,i/*当たった相手のid*/,0/*ダミー*/);
        }
        printf("%s\n",data);
        
        SendData(data);
    
}
/**************************************************
当たり判定  clientID:自分のID 
            befx,befy:前のx座標y座標
返し値      0       :あたってない
           1       :あたった
***************************************************/
int Collision(int clientID,int befx,int befy){

    int i;
    //グローバル変数で全体人数を設定

    switch(gClients[clientID].ADsta){
    case 0:
        for(i=0;i<cnum;i++){
            if(i != clientID){
                if((gClients[i].poi.x - gClients[clientID].poi.x) <= 30 && (gClients[clientID].poi.x - gClients[i].poi.x) <= 30){
                    if((gClients[i].poi.y - gClients[clientID].poi.y) <= 30 && (gClients[clientID].poi.y - gClients[i].poi.y) <= 30){
                        gClients[clientID].poi.x = befx;
                        gClients[clientID].poi.y = befy;
                        //printf("color\n");
                        if(gClients[i].ADsta==1){
                            gClients[clientID].Bflag++;//自分に当たり判定のフラグを立てる
                            gClients[clientID].color=2;/*i*/
                        }
                    }
                }
            }
        }
        break;

    case 1:
        for(i=0;i<cnum;i++){
            if(i != clientID){
                if((gClients[i].poi.x - gClients[clientID].poi.x) <= 30 && (gClients[clientID].poi.x - gClients[i].poi.x) <= 30){
                    if((gClients[i].poi.y - gClients[clientID].poi.y) <= 30 && (gClients[clientID].poi.y - gClients[i].poi.y) <= 30){
                        //printf("color\n");
                        gClients[clientID].poi.x = befx;
                        gClients[clientID].poi.y = befy;
                    }
                }

                if((gClients[i].poi.x - (gClients[clientID].poi.x-20)) <= 70 && ((gClients[clientID].poi.x-20) - gClients[i].poi.x) <= 70){
                    if((gClients[i].poi.y - (gClients[clientID].poi.y-20)) <= 70 && ((gClients[clientID].poi.y-20) - gClients[i].poi.y) <= 70){
                        printf("i=%d,client=%d\n",gClients[i].Bflag,gClients[clientID].Bflag);
                        if(gClients[i].Bflag==0){
                            printf("BUMP\n");
                            gClients[clientID].Bflag++;
                            gClients[i].Bflag++;
                            gClients[clientID].score++;
                            gClients[clientID].color=3;
                            gClients[i].color=2;
                            return i;
                        }
                    }
                }
            }
        }
        break;
    }    

    //printf("Collision\n");

    return -1;
}
