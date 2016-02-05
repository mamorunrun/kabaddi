#include"common.h"
#include"client_func.h"


void UpdatePos(int n,int x,int y,int t,int rect_x,int rect_y,int rect_w)
{
    if(clientID == n)
        return;
    if(t >= 0){//攻撃側は時間を,守備側は-1を送っているため
        game.restTime = t;
        /*以下nは攻撃を指す*/
        if(Af == 1){
            Ax = gClients[n].poi.x; 
            Ay = gClients[n].poi.y;
            Af = 2;
            printf("changeAx,Ay:%d,%d\n",Ax,Ay);
        }
    }
    gClients[n].poi.x=x;
    gClients[n].poi.y=y;
    chara_rect[n].x=rect_x;
    chara_rect[n].y=rect_y;
    chara_rect[n].w=rect_w;
}

/*********************************************************
自分のキャラの移動と当たり判定をサーバーに送信
自分と攻撃手以外にもタッチされたことを伝える
                   clientID :自分のID
                   cnum     :クライアントの数
                   befx,befy:前のx座標y座標
当たり判定（壁,キャラ）があればキャラの座標を戻す
*********************************************************/
void Move(int clientID,int befx,int befy,int now)
{
    char	data[MAX_DATA];
    int			dataSize = 0;
    int *x,*y;
    int i = -1;
    int end=1;
    
    //printf("Move\n");

    switch(gClients[clientID].ADsta){
    case 0:
        if(gClients[clientID].poi.x <= 0)
            gClients[clientID].poi.x = 0;
        else if(gClients[clientID].poi.x + 96 >= 800)
            gClients[clientID].poi.x = 800 - 96;
        if(gClients[clientID].poi.y <= 0)
            gClients[clientID].poi.y = 0;
        else if(gClients[clientID].poi.y + 144 >= 600)
            gClients[clientID].poi.y = 600 -144;
        break;
    case 1:
        if(gClients[clientID].poi.x <= 0)
            gClients[clientID].poi.x = 0;
        else if(gClients[clientID].poi.x + 96 >= 1000)
            gClients[clientID].poi.x = 1000 - 96;
        //       if(gClients[clientID].poi.x >= 850){
        //  end=0;
        //   if(gClients[clientID].Bflag > 0){
        //      sprintf(data,"kabaddi,%d,%d,%d,%d\0",WIN,clientID,i,0);
        //      SendData(data);
        //  }
        // }

        if(gClients[clientID].poi.y <= 0)
            gClients[clientID].poi.y = 0;
        else if(gClients[clientID].poi.y + 144 >= 600)
            gClients[clientID].poi.y = 600 -144;
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
    // printf("%d\n",i);        

    Animation(now); 

     printf("anime =%d %d\n",chara_rect[clientID].x,chara_rect[clientID].y);
    
    if( i == -1){
        if(gClients[clientID].ADsta == 1)
            sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d,%d\0",CDRAW,clientID,gClients[clientID].poi.x,gClients[clientID].poi.y,game.restTime,chara_rect[clientID].x,chara_rect[clientID].y,chara_rect[clientID].w);
        else
            sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d,%d\0",CDRAW,clientID,gClients[clientID].poi.x,gClients[clientID].poi.y,-1,chara_rect[clientID].x,chara_rect[clientID].y,chara_rect[clientID].w/*ダミー*/);
        SendData(data);
    }
    //else if(gClients[clientID].ADsta == 1){
        //sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d\0",WIN,clientID,i/*当たった相手(守備)のid*/,0/*ダミー*/,0,0,0);
//SendData(data);
//  }
// else {
//      sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d\0",WIN,i/*当たった相手(攻撃)のid*/,clientID,0/*ダミー*/,0,0,0);
//      SendData(data);
//  }
    printf("%s\n",data);
    
    if(gClients[clientID].ADsta == 1){
        if(gClients[clientID].poi.x+96 >= 850){
            //end=0;
            //if(gClients[clientID].Bflag > 0){
            sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d,%d\0",SCORE,clientID,gClients[clientID].score,0,0,0,0,0);
                SendData(data);
                // }
        }
    }
    
    if(end == 0){
        sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d,%d\0",END_COMMAND,0,0,0,0,0,0,0);
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
    int touchx,touchy;
    //グローバル変数で全体人数を設定

    switch(gClients[clientID].ADsta){
    case 0://守備
        for(i=0;i<cnum;i++){
            if(i != clientID){
                if(((gClients[i].poi.x+33) - (gClients[clientID].poi.x+33)) <= 30 && ((gClients[clientID].poi.x+33) - (gClients[i].poi.x+33)) <= 30){
                    if(((gClients[i].poi.y+57) - (gClients[clientID].poi.y+57)) <= 30 && ((gClients[clientID].poi.y+57) - (gClients[i].poi.y+57)) <= 30){
                        gClients[clientID].poi.x = befx;
                        gClients[clientID].poi.y = befy;
                    }
                }
            }
            
            //printf("color\n");
            if(gClients[i].ADsta==1){//相手が攻撃なら
                /*if(gClients[clientID].poi.x+33 < gClients[i].poi.x+33+gClients[i].poi.w-66+20){
                  if(gClients[clientID].poi.y+57 < gClients[i].poi.y+57+gClients[i].poi.h-114+20){
                  if(gClients[clientID].poi.x+33 + gClients[clientID].poi.w-66 > gClients[i].poi.x+33-20){
                  if(gClients[clientID].poi.y+57 + gClients[clientID].poi.h-114 > gClients[i].poi.y+57-20){
                */
                
                if(gClients[clientID].tackle == 0){//攻守反転していて
                    if(tflag >=1 && tflag <= 10){//自分がタックルしてたら
                        
                        if(dirflag == up_dir || dirflag == down_dir){
                            if(dirflag == up_dir)
                                touchy = Tauy;
                            else if(dirflag == down_dir)
                                touchy = Tady;
                            if(gClients[clientID].poi.x + Taudx       < gClients[i].poi.x + Dfx + Dfw){
                                if(gClients[clientID].poi.x + Taudx + Taudw > gClients[i].poi.x + Dfx){
                                    if(gClients[clientID].poi.y + touchy       < gClients[i].poi.y + Dfy + Dfh){
                                        if(gClients[clientID].poi.y + touchy + Taudh > gClients[i].poi.y + Dfy){
                                            sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d,%d\0",TACKLE,i/*当たった相手(攻撃)のid*/,clientID,0/*ダミー*/,0,0,0,0);
                                            gClients[clientID].tackle = 1;/*自分にもフラグを*/
                                            if(Af == 0){
                                                Af = 1;
                                            }
                                            SendData(data);
                                        }
                                    }
                                }
                            }
                        }
                        
                        else{// dirflag != up_dir,down_dir
                            if(dirflag == up_right_dir || dirflag == right_dir || dirflag ==right_down_dir)
                                touchx = Tarx;
                            else if(dirflag == down_left_dir || dirflag == left_dir || dirflag == left_up_dir)
                                touchx = Talx;
                            
                            if(gClients[clientID].poi.x + touchx       < gClients[i].poi.x + Dfx + Dfw){
                                if(gClients[clientID].poi.x + touchx + Talrw > gClients[i].poi.x + Dfx){
                                    if(gClients[clientID].poi.y + Talry       < gClients[i].poi.y + Dfy + Dfh){
                                        if(gClients[clientID].poi.y + Talry + Talrh > gClients[i].poi.y + Dfy){
                                            sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d,%d\0",TACKLE,i/*当たった相手(攻撃)のid*/,clientID,0/*ダミー*/,0,0,0,0);
                                            gClients[clientID].tackle = 1;/*自分にもフラグを*/
                                            if(Af == 0){
                                                Af = 1;
                                            }
                                            SendData(data);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            
                if(gClients[clientID].poi.x + Dfx       < gClients[i].poi.x + Atx + Atw){
                    if(gClients[clientID].poi.x + Dfx + Dfw > gClients[i].poi.x + Atx){
                        if(gClients[clientID].poi.y + Dfy       < gClients[i].poi.y + Aty + Ath){
                            if(gClients[clientID].poi.y + Dfy + Dfh > gClients[i].poi.y + Aty){
                                if(gClients[clientID].Bflag==0){//自分(守備)に当たり判定がなければ
                                        
                                        // gClients[i].Bflag++;//自分に当たり判定のフラグを立てる
                                        // gClients[clientID].Bflag++;//攻撃側にフラグ
                                        //gClients[i].color=3;//攻撃
                                        //gClients[clientID].color=2;//守備
                                    
                                    sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d,%d\0",BUMP,i/*当たった相手(攻撃)のid*/,clientID,0/*ダミー*/,0,0,0,0);
                                    SendData(data);
                                    
                                    return i;//攻撃
                                }
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
                if(((gClients[i].poi.x+33) - (gClients[clientID].poi.x+33)) <= 30 && ((gClients[clientID].poi.x+33) - (gClients[i].poi.x+33)) <= 30){
                    if(((gClients[i].poi.y+57) - (gClients[clientID].poi.y+57)) <= 30 && ((gClients[clientID].poi.y+57) - (gClients[i].poi.y+57)) <= 30){
                        //printf("color\n");
                        gClients[clientID].poi.x = befx;
                        gClients[clientID].poi.y = befy;
                    }
                }
                /*if(gClients[i].poi.x+33 < gClients[clientID].poi.x+33+gClients[clientID].poi.w-66+20){
                    if(gClients[i].poi.y+57 < gClients[clientID].poi.y+57+gClients[clientID].poi.h-114+20){
                        if(gClients[i].poi.x+33 + gClients[i].poi.w-66 > gClients[clientID].poi.x+33-20){
                        if(gClients[i].poi.y+57+gClients[i].poi.h-144 > gClients[clientID].poi.y+57-20){*/
                if(gClients[i].poi.x+ Dfx < gClients[clientID].poi.x+ Atx + Atw){
                    if(gClients[i].poi.x + Dfx + Dfw > gClients[clientID].poi.x + Atx){
                        if(gClients[i].poi.y + Dfy < gClients[clientID].poi.y+ Aty+ Ath){
                            if(gClients[i].poi.y + Dfy +Dfh > gClients[clientID].poi.y + Aty){
                                
                                
                                if(gClients[i].Bflag==0)//相手(守備)にフラグがなければ
                                {
                                    // gClients[clientID].Bflag++;
                                    // gClients[i].Bflag++;
                                    //gClients[clientID].color=3;//攻撃
                                    //gClients[i].color=2;//守備
                                    sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d,%d\0",BUMP,clientID/*当たった相手(攻撃)のid*/,i,0/*ダミー*/,0,0,0,0);
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
            }
        }
            break;
    }    

    //printf("Collision\n");

    return -1;
}

void Animation(int now){



    if(now >= gClients[clientID].anime){
        
        if(tflag >=1 && gClients[clientID].ADsta==0){
            if(dirflag != up_dir && dirflag != down_dir){
                if(chara_rect[clientID].x <768){
                    printf("tackle\n");
                    printf("%d   %d   %d  \n\n",chara_rect[clientID].x,chara_rect[clientID].y,chara_rect[clientID].w );
                    
                    chara_rect[clientID].x += 192;
                }
            }
        }
        
        else if(dirflag == up_dir || dirflag == down_dir){
            if(gClients[clientID].anipatnum<6){
                gClients[clientID].ADsta ? (chara_rect[clientID].y=0) : (chara_rect[clientID].y=144);
                gClients[clientID].anipatnum++;
                chara_rect[clientID].x += 96;
            }
        }

        else if(dirflag == right_dir || dirflag == left_dir){
            printf("right \n\n");

            if(gClients[clientID].anipatnum<6){
                gClients[clientID].ADsta ? (chara_rect[clientID].y=288) : (chara_rect[clientID].y=432);
                gClients[clientID].anipatnum++;
                chara_rect[clientID].x += 96;
            }
        }

        else if(dirflag == up_right_dir || dirflag == down_left_dir){
            if(gClients[clientID].anipatnum<6){
                gClients[clientID].ADsta ? (chara_rect[clientID].y=288) : (chara_rect[clientID].y=720);
                gClients[clientID].anipatnum++;
                chara_rect[clientID].x += 96;
            }
        }
        
        else if(dirflag == left_up_dir || dirflag == right_down_dir){
            if(gClients[clientID].anipatnum<6){
                gClients[clientID].ADsta ? (chara_rect[clientID].y=576) : (chara_rect[clientID].y=432);
                gClients[clientID].anipatnum++;
                chara_rect[clientID].x += 96;
            }
        }

        printf("color=%d\n",gClients[clientID].color);

        if(gClients[clientID].anipatnum >=6){
            gClients[clientID].anipatnum=0;
            if(gClients[clientID].color==4)
                chara_rect[clientID].x=576;
            else
                chara_rect[clientID].x=0;
        }

        gClients[clientID].anime=now+25;

    }

}
