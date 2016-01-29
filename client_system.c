#include"common.h"
#include"client_func.h"


void UpdatePos(int n,int x,int y,int t)
{
    if(clientID == n)
        return;

    gClients[n].poi.x=x;
    gClients[n].poi.y=y;
    
    if(t >= 0)//攻撃側は時間を,守備側は-1を送っているため
        game.restTime = t;
}

/*********************************************************
自分のキャラの移動と当たり判定をサーバーに送信
自分と攻撃手以外にもタッチされたことを伝える
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
    int end=1;

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
        //       if(gClients[clientID].poi.x >= 850){
        //  end=0;
        //   if(gClients[clientID].Bflag > 0){
        //      sprintf(data,"kabaddi,%d,%d,%d,%d\0",WIN,clientID,i,0);
        //      SendData(data);
        //  }
        // }

        if(gClients[clientID].poi.y <= 0)
            gClients[clientID].poi.y = 0;
        else if(gClients[clientID].poi.y + 30 >= 600)
            gClients[clientID].poi.y = 600 -30;
        break;
        
    }

    i=Collision(clientID,befx,befy);
    //あたった相手のidが入る

    /*
    if(gClients[clientID].Bflag > 0){
        if(gClients[clientID].poi.x >= 600){
            WIN,LOSEフラグの送信
              
              sprintf(data,"kabaddi,%d,%d,%d,%d\0",LOSE,clientID,i,0);
              SendData(data);
              
              
              win();
              break;
            
            
        }
    } */   
    printf("%d\n",i);        
    
    if( i == -1){
        if(gClients[clientID].ADsta == 1)
            sprintf(data,"kabaddi,%d,%d,%d,%d,%d\0",CDRAW,clientID,gClients[clientID].poi.x,gClients[clientID].poi.y,game.restTime);
        else
            sprintf(data,"kabaddi,%d,%d,%d,%d,%d\0",CDRAW,clientID,gClients[clientID].poi.x,gClients[clientID].poi.y,-1/*ダミー*/);
        SendData(data);
    }
    else if(gClients[clientID].ADsta == 1){
        sprintf(data,"kabaddi,%d,%d,%d,%d,%d\0",WIN,clientID,i/*当たった相手(守備)のid*/,0/*ダミー*/,0);
        SendData(data);
    }
    else {
        sprintf(data,"kabaddi,%d,%d,%d,%d,%d\0",WIN,i/*当たった相手(攻撃)のid*/,clientID,0/*ダミー*/,0);
        SendData(data);
    }
    printf("%s\n",data);
    
    if(gClients[clientID].ADsta == 1){
        if(gClients[clientID].poi.x >= 850){
            //end=0;
            if(gClients[clientID].Bflag > 0){
                sprintf(data,"kabaddi,%d,%d,%d,%d,%d\0",WIN,clientID,i,0,0);
                SendData(data);
            }
        }
    }
    
    if(end == 0){
        sprintf(data,"kabaddi,%d,%d,%d,%d,%d\0",END_COMMAND,0,0,0,0);
        SendData(data);
    }
}
/**************************************************
当たり判定  clientID:自分のID 
            befx,befy:前のx座標y座標
返し値      0       :あたってない
           1       :あたった
***************************************************/
int Collision(int clientID,int befx,int befy){
    char	data[MAX_DATA];

    int i,j;
    //グローバル変数で全体人数を設定

    switch(gClients[clientID].ADsta){
    case 0:
        for(i=0;i<cnum;i++){
            if(i != clientID){
                if((gClients[i].poi.x - gClients[clientID].poi.x) <= 30 && (gClients[clientID].poi.x - gClients[i].poi.x) <= 30){
                    if((gClients[i].poi.y - gClients[clientID].poi.y) <= 30 && (gClients[clientID].poi.y - gClients[i].poi.y) <= 30){
                        gClients[clientID].poi.x = befx;
                        gClients[clientID].poi.y = befy;
                    }
                }
                        //printf("color\n");
                if(gClients[i].ADsta==1){//相手が攻撃なら
                    if((gClients[clientID].poi.x - (gClients[i].poi.x-20)) <= 70 && ((gClients[i].poi.x-20) - gClients[clientID].poi.x) <= 70){//大きめの範囲で
                        if((gClients[clientID].poi.y - (gClients[i].poi.y-20)) <= 70 && ((gClients[i].poi.y-20) - gClients[clientID].poi.y) <= 70){
                            if(gClients[clientID].Bflag==0)//自分(守備)に当たり判定がなければ
                            {
                                // gClients[i].Bflag++;//自分に当たり判定のフラグを立てる
                                // gClients[clientID].Bflag++;//攻撃側にフラグ
                                //gClients[i].color=3;//攻撃
                                //gClients[clientID].color=2;//守備

                                sprintf(data,"kabaddi,%d,%d,%d,%d,%d\0",BUMP,i/*当たった相手(攻撃)のid*/,clientID,0/*ダミー*/,0);
                                SendData(data);

                                return i;//攻撃
                            }
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
                if((gClients[i].poi.x - gClients[clientID].poi.x + 20) <= 70 && (gClients[clientID].poi.x - 20 - gClients[i].poi.x <= 70){
                    if((gClients[i].poi.y - gClients[clientID].poi.y + 20) <= 70 && (gClients[clientID].poi.y - 20 - gClients[i].poi.y) <= 70){
                        if(gClients[i].Bflag==0)//相手(守備)にフラグがなければ
                        {
                            // gClients[clientID].Bflag++;
                            // gClients[i].Bflag++;
                            //gClients[clientID].color=3;//攻撃
                            //gClients[i].color=2;//守備

                            sprintf(data,"kabaddi,%d,%d,%d,%d,%d\0",BUMP,clientID/*当たった相手(攻撃)のid*/,i,0/*ダミー*/,0);
                            SendData(data);

                            return i;//守備
                        //printf("%d\n",gClients[i].Bflag,gClients[clientID].Bflag);
                        // if(gClients[i].Bflag==0){
                        // printf("BUMP\n");
                        //  gClients[clientID].Bflag++;
                            //gClients[i].Bflag++;
                            //gClients[clientID].score++;
                            //gClients[clientID].color=3;
                            //gClients[i].color=2;
                            //return i;
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
