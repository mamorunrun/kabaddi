#include"common.h"
#include"client_func.h"


/*******************************************************************
各クライアントの座標を更新 
 *******************************************************************/
void UpdatePos(int n,int x,int y,int t,int rect_x,int rect_y,int rect_w)
{
    if(clientID == n)//自分の情報なら何もしない
        return;
    if(t >= 0){//攻撃側は自分のgame.restTimeを,守備側は-1を送っているため

        game.restTime = t;//攻撃側のgame.restTimeを守備側は取得
        /*以下nは攻撃側のIDを指す*/
        if(Af == 1){
            Ax = gClients[n].poi.x; 
            Ay = gClients[n].poi.y;
            Af = 2;
            //守備側がタックルに成功した時に攻撃側に引きずられる動作のため
        }
    }

    //以下攻撃,守備にかかわらず座標の更新
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
    char	data[MAX_DATA];//コマンド用
    
    int i = -1;//当たり判定用
    
    switch(gClients[clientID].ADsta){//移動できる範囲の設定
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
        if(gClients[clientID].poi.y <= 0)
            gClients[clientID].poi.y = 0;
        else if(gClients[clientID].poi.y + 144 >= 600)
            gClients[clientID].poi.y = 600 -144;
        break;
        
    }

    i=Collision(clientID,befx,befy);
    //あたった相手のidが入る
    
    Animation(now);//アニメーションの更新

     if(i == -1){//誰とも衝突が無かった
         if(gClients[clientID].ADsta == 1)//攻撃側は時間を送る
            sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d,%d\0",CDRAW,clientID,gClients[clientID].poi.x,gClients[clientID].poi.y,game.restTime,chara_rect[clientID].x,chara_rect[clientID].y,chara_rect[clientID].w);
        else
            sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d,%d\0",CDRAW,clientID,gClients[clientID].poi.x,gClients[clientID].poi.y,-1,chara_rect[clientID].x,chara_rect[clientID].y,chara_rect[clientID].w/*ダミー*/);
        SendData(data);
    }
    
    if(gClients[clientID].ADsta == 1){
        if(gClients[clientID].poi.x+96 >= 850){//攻撃側が線を超えた時に得点を追加
            sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d,%d\0",SCORE,clientID,gClients[clientID].score,0,0,0,0,0);
                SendData(data);
        }
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
    int touchx,touchy;//タックル時の当たり判定のため
    
    switch(gClients[clientID].ADsta){
    case 0://自分が守備
        for(i=0;i<cnum;i++){
            if(i != clientID){
                
/***************************************************************************************************************
 当たり判定,キャラの大きさが縦144,横96に対して中心の30*30の四角形で判定するため
 以下Dから始まる値は守備が持つ当たり判定,
    Aから始まる値は攻撃が持つ当たり判定,
    Taから始まる値はタックル時のため
BUMPコマンド 攻撃と守備の当たり判定のみ TACLEコマンド タックルの成功時
***************************************************************************************************************/
                
                if(gClients[clientID].poi.x + Dfx       < gClients[i].poi.x + Dfx + Dfw){
                    if(gClients[clientID].poi.x + Dfx + Dfw > gClients[i].poi.x + Dfx){
                        if(gClients[clientID].poi.y + Dfy       < gClients[i].poi.y + Dfy + Dfh){
                            if(gClients[clientID].poi.y + Dfy + Dfh > gClients[i].poi.y + Dfy){
                                
                                gClients[clientID].poi.x = befx;
                                gClients[clientID].poi.y = befy;
                                //座標を元に戻す
                            }
                        }
                    }
                }
                
                if(gClients[i].ADsta==1){//相手が攻撃なら
                    if(gClients[clientID].tackle == 0){//攻守反転していて
                        if(dirflag == up_dir || dirflag == down_dir){
                            if(tflag >=1 && tflag <= 10){//自分がタックルしてたら
                                if(dirflag == up_dir)
                                    touchy = Tauy;
                                else if(dirflag == down_dir)//当たり判定の設定
                                    touchy = Tady;
                                /*タックルの成功,守備→攻撃への当たり判定,*/
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
                            
                            else if(tflag <= 200){//タックルの硬直時間,当たり判定のみ
                                if(gClients[clientID].poi.x + Taudx       < gClients[i].poi.x + Atx + Atw){
                                    if(gClients[clientID].poi.x + Taudx + Taudw > gClients[i].poi.x + Atx){
                                        if(gClients[clientID].poi.y + touchy       < gClients[i].poi.y + Aty + Ath){
                                            if(gClients[clientID].poi.y + touchy + Taudh > gClients[i].poi.y + Aty){
                                                if(gClients[clientID].Bflag==0){//自分(守備)に当たり判定がなければ
                                                    
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
                        else{// dirflag != up_dir,down_dir
                            if(tflag >=1 && tflag <= 10){//自分がタックルしてたら
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
                            else if(tflag <= 200){
                                if(gClients[clientID].poi.x + touchx       < gClients[i].poi.x + Atx + Atw){
                                    if(gClients[clientID].poi.x + touchx + Talrw > gClients[i].poi.x + Atx){
                                        if(gClients[clientID].poi.y + Talry       < gClients[i].poi.y + Aty + Ath){
                                            if(gClients[clientID].poi.y + Talry + Talrh > gClients[i].poi.y + Aty){
                                                if(gClients[clientID].Bflag==0){//自分(守備)に当たり判定がなければ
                                                    
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
                    }
                
                        //タックル以外で守備と攻撃
                    if(gClients[clientID].poi.x + Dfx       < gClients[i].poi.x + Atx + Atw){
                        if(gClients[clientID].poi.x + Dfx + Dfw > gClients[i].poi.x + Atx){
                            if(gClients[clientID].poi.y + Dfy       < gClients[i].poi.y + Aty + Ath){
                                if(gClients[clientID].poi.y + Dfy + Dfh > gClients[i].poi.y + Aty){
                                    if(gClients[clientID].Bflag==0){//自分(守備)に当たり判定がなければ
                                        
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
        }
    
            
        break;
            
        
    case 1:
        for(i=0;i<cnum;i++){
            if(i != clientID){
                if(gClients[clientID].poi.x + Dfx       < gClients[i].poi.x + Dfx + Dfw){
                    if(gClients[clientID].poi.x + Dfx + Dfw > gClients[i].poi.x + Dfx){
                        if(gClients[clientID].poi.y + Dfy       < gClients[i].poi.y + Dfy + Dfh){
                            if(gClients[clientID].poi.y + Dfy + Dfh > gClients[i].poi.y + Dfy){
                                gClients[clientID].poi.x = befx;
                                gClients[clientID].poi.y = befy;
                            }
                        }
                    }
                }
                
                if(gClients[i].poi.x+ Dfx < gClients[clientID].poi.x+ Atx + Atw){
                    if(gClients[i].poi.x + Dfx + Dfw > gClients[clientID].poi.x + Atx){
                        if(gClients[i].poi.y + Dfy < gClients[clientID].poi.y+ Aty+ Ath){
                            if(gClients[i].poi.y + Dfy +Dfh > gClients[clientID].poi.y + Aty){
                                    
                                if(gClients[i].Bflag==0){//相手(守備)にフラグがなければ  
                                    sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d,%d\0",BUMP,clientID/*当たった相手(攻撃)のid*/,i,0/*ダミー*/,0,0,0,0);
                                    SendData(data);   
                                    return i;//守備
                                }
                            }
                        }
                    }
                }
            }
        }
        
        break;
    }
    return -1;
}

/*アニメーションの関数*/
void Animation(int now){


    /*キャラの向きや、タックルによって描写を変える*/
    /*現在時間nowが指定した時間を超えたら貼り付ける画像を変える*/
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
        /*次の描写の時間を指定する*/
        gClients[clientID].anime=now+25;

    }

}
