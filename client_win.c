/*****************************************************************
ファイル名	: client_win.c
機能		: クライアントのユーザーインターフェース処理
*****************************************************************/

#include<SDL/SDL.h>
#include<SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include<SDL/SDL_gfxPrimitives.h>
#include"common.h"
#include"client_func.h"

static SDL_Surface *gMainWindow;
static SDL_Surface *buffer;
static SDL_Surface *scbuf;//得点を表すバッファ
static SDL_Surface *stbar;//スタミナを表すバッファ
static SDL_Surface *gCharaImage;
static char gPlayerImgFile[]   = "kabaddi.png";

SDL_Rect chara_rect[MAX_CLIENTS];
static SDL_Surface *bufmain;//スタート画面,終了画面など
SDL_Rect STrect = {0, 0, 0, 50};//スタミナゲージのため
SDL_Rect srect = {400,0};//stbarの領域
SDL_Rect brect = {0, 51};//bufferからの領域
static int cID;
CLIENT gClients[MAX_CLIENTS];

void DisplayStatus(void);
int Ax,Ay;//攻撃の移動
int tflag;//タックルのフラグ

int dflag;//グローバル変数,攻守反転
int dirflag;//方向を表す

int resultflag;//自分以外の人の結果を確認するため
int continueflag;//連続入力の破棄

int color[5] = {0x0000ffff,0xff0000ff,0x00ff00ff,0xff00ffff,0x00ff00ff};//2と4はcommand.c内l.65のため同じ
int stcol[3] = {0x0000ff,0xffff00,0xff0000};//青,黄色,赤
SDL_Color colB = {0,0,0};//黒色（文字）

char Pname[MAX_CLIENTS][MAX_NAME_SIZE+2];//キャラの名前
SDL_Surface *PNAME[MAX_CLIENTS];//キャラの名前用サーフェイス
SDL_Rect PNAME_srect[MAX_CLIENTS];//そのための四角形
SDL_Rect PNAME_rrect[MAX_CLIENTS];//そのための四角形

static TTF_Font* font;	// TrueTypeフォントデータへのポインタ
static TTF_Font* font2;
static TTF_Font* font3;
static TTF_Font* Font;//DisplayStatus

 /*時間描画のためstatic*/
static SDL_Rect Src_rect;//時間描画
static SDL_Rect Dst_rect = {0,0};//転送先
static SDL_Surface *mes;
/*****************************************************************
関数名	: InitWindows
機能	: メインウインドウの表示，設定を行う
引数	: なし
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
int InitWindows(void)
{
    cID = clientID;
    int i;
    char *s,title[10];
    int end = 1;

    SDL_Event event;
    SDL_Rect dst_rect = { 350, 250 };
    SDL_Rect dst_rect2 = { 370, 350 };
    SDL_Rect dst_rect3 = { 250, 280 };
//    SDL_Rect src_rect3 = { 100, 400, gMessage->w, gMessage->h };
    SDL_Surface *gMessage_title;
    SDL_Surface *gMessage_req;
    SDL_Surface *gMessage_chotomate;
    
    /* SDLの初期化 */
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("failed to initialize SDL.\n");
        return -1;
    }
    
    /* メインのウインドウを作成する */
    if((gMainWindow = SDL_SetVideoMode(1000,650, 32, SDL_SWSURFACE)) == NULL) {
        printf("failed to initialize videomode.\n");
        return -1;
    }
    if((bufmain = SDL_SetVideoMode(1000,650, 32, SDL_SWSURFACE)) == NULL) {
        printf("failed to initialize videomode.\n");
        return -1;
    }
    if((buffer = SDL_CreateRGBSurface(SDL_SWSURFACE,1000,600,32,0,0,0,0))==NULL){
        printf("failed to initialize videomode.\n");
        exit(-1);
    }

     if((scbuf = SDL_CreateRGBSurface(SDL_SWSURFACE,400, 50,32,0,0,0,0))==NULL){
        printf("failed to initialize videomode.\n");
        exit(-1);
    }
     if((stbar = SDL_CreateRGBSurface(SDL_SWSURFACE,600, 50,32,0,0,0,0))==NULL){
        printf("failed to initialize videomode.\n");
        exit(-1);
    }

    if((gCharaImage= IMG_Load(gPlayerImgFile)) ==  NULL){
        printf("failed to open player image.");
        exit(-1);
    }


    /* フォントの初期化 */
    TTF_Init();

    font = TTF_OpenFont("kochi-gothic-subst.ttf",48); // フォントの設定kochi-gothic-substフォントを48ポイントで使用（読み込み）
    font2 = TTF_OpenFont("kochi-gothic-subst.ttf",24);//得点の描写
    font3 = TTF_OpenFont("kochi-gothic-subst.ttf",12);//名前用
    /* ウインドウのタイトルをセット */
    sprintf(title,"Kabaddi[%d]",clientID);
    SDL_WM_SetCaption(title,NULL);
    
    /* 背景を白にする */
    SDL_FillRect(bufmain,NULL,0xffffffff);

    gMessage_title = TTF_RenderUTF8_Blended(font, "白熱カバッディ", /*0x191970ff*/colB);
    SDL_Rect src_rect = { 0, 0, gMessage_title->w,gMessage_title->h };
    gMessage_req = TTF_RenderUTF8_Blended(font2, "スペースキーを押すんだ！", /*0x191970ff*/colB);
    SDL_Rect src_rect2 = { 0, 0, gMessage_req->w,gMessage_req->h  };

    SDL_BlitSurface(gMessage_title, &src_rect, bufmain, &dst_rect);
    SDL_BlitSurface(gMessage_req, &src_rect2, bufmain, &dst_rect2);

    SDL_BlitSurface(bufmain, NULL, gMainWindow, &brect);
    SDL_Flip(gMainWindow);

    while(end){
        if(SDL_PollEvent(&event)){// イベント取得ができた場合イベントにより処理を分岐
            switch(event.type){
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                case SDLK_SPACE:
                    end = 0;
                    break;
                }
            }
        }
    }
        printf("aaa\n");

        /* 背景を白にする */
        SDL_FillRect(bufmain,NULL,0xffffffff);

        gMessage_chotomate = TTF_RenderUTF8_Blended(font, "ちょっと待ってくれい！", /*0x000000ff*/colB);
    SDL_Rect src_rect3 = { 0, 0, gMessage_chotomate->w,gMessage_chotomate->h  }; 
        SDL_BlitSurface(gMessage_chotomate, &src_rect3, bufmain, &dst_rect3);   

    SDL_BlitSurface(bufmain, NULL, gMainWindow, &brect);
    SDL_Flip(gMainWindow);
    
    return 0;
}

/*****************************************************************
関数名	: GameWindows
機能	: ゲームウインドウの表示，設定を行う
引数	: int	clientID		: クライアント番号
	  char  name                    : 名前
          int   loop                    : 何回目のループか	
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
int GameWindows(int clientID,char name[][MAX_NAME_SIZE], int loop)
{
    
     /* 引き数チェック */
        assert(0<num && cnum<=MAX_CLIENTS);
	
/*　　　文字関係
        char Pname[cnum][MAX_NAME_SIZE+2];
        SDL_Surface *PNAME[cnum];
*/      

        dflag = 0;/*攻守反転フラグの初期化*/
        tflag = 0;
        game.restTime = 20000;/*残り20000ミリ（20）秒*/
        lineColor(buffer, 800, 0, 800, 600,0x000000ff);
        /*始点x座標，始点y座標，終点x座標，終点y座標，色*/

	/* 背景を白にする */
	SDL_FillRect(buffer,NULL,0xffffffff);
       
        int i,x,y;

        for(i=0;i<cnum;i++){
            if(i == (loop % cnum)){
                gClients[i].poi.x=700;
                gClients[i].poi.y=250;
                gClients[i].ADsta = 1;/*最初は攻撃*/
                gClients[i].color=1;
                cID = i;
            }
            else{
                gClients[i].poi.x=200;
                gClients[i].poi.y=100 + i*200;
                chara_rect[i].y=144;
                gClients[i].ADsta = 0;/*最後二人は守備*/
                gClients[i].color=0;
            }
            gClients[i].anipatnum=0;
            gClients[i].anime=100;
            gClients[i].poi.w=30;
            gClients[i].poi.h=30;
            gClients[i].tackle = 0;
            chara_rect[i].w=96;
            chara_rect[i].h=144;
            /*if(gClients[i].ADsta==1){
                rectangleColor(buffer,gClients[i].poi.x-20,gClients[i].poi.y-20,gClients[i].poi.x+50,gClients[i].poi.y+50,0x000000ff);
                }*/

            //printf("%d,%d,%d\n",i,gClients[i].poi.x,gClients[i].poi.y);
            gClients[i].Bflag = 0;
            SDL_BlitSurface(gCharaImage,&chara_rect[i],buffer,&gClients[i].poi);
/******************四角の上に文字を出力 SDL_BlitSurfaceの活用************************/
            sprintf(Pname[i],"     ▼%s",gClients[i].name);
            PNAME[i] = TTF_RenderUTF8_Blended(font3,Pname[i],colB);
            PNAME_srect[i].w = PNAME[i]->w;
            PNAME_srect[i].h = PNAME[i]->h ;
            PNAME_rrect[i].x = gClients[i].poi.x;
            PNAME_rrect[i].y = gClients[i].poi.y - 5;
            SDL_BlitSurface(PNAME[i], &PNAME_srect[i], buffer,&PNAME_rrect[i]);
          
            

        }
        printf("loop=%d\n",loop);

/*******************************************************************************
　　　　　　得点の描写
 ****************************************************************************/
        char   status[64];

        sprintf(status,"score:%dpt",gClients[clientID].score);
        //printf("%s\n",status);
    
        mes = TTF_RenderUTF8_Blended(font2, status, colB);
  
    // 背景を白にする 
        SDL_FillRect(scbuf,NULL,0xffffffff);
        SDL_BlitSurface(mes, NULL, scbuf, NULL);
        SDL_BlitSurface(scbuf, NULL, gMainWindow, NULL);
        
        SDL_BlitSurface(buffer, NULL, gMainWindow, &brect);
	SDL_Flip(gMainWindow);
        game.flag = 2;

	return 0;
}
/*****************************************************************
関数名	: DestroyWindow
機能	: SDLを終了する
引数	: なし
出力	: なし
*****************************************************************/
void DestroyWindow(void)
{
    TTF_CloseFont(font);
    TTF_CloseFont(font2);
    SDL_Quit();
}

void WindowEvent(int clientID,int now)
{
    int a = 2;
    int mflag = 1;//moveflag
    int befx,befy;
    
    char	data[MAX_DATA];
    befx = gClients[clientID].poi.x;
    befy = gClients[clientID].poi.y;

    while (wiimote_is_open(&wiimote)){
        if (wiimote_update(&wiimote) < 0) {
            wiimote_disconnect(&wiimote);
            break;
        }

        if (wiimote.keys.home) {
            printf("home\n");
            wiimote_speaker_free(&wiimote);
            wiimote_disconnect(&wiimote);
            game.flag = 0;
            SendEndCommand();
        }

        if(game.flag == 2){//ゲームフラグが2以外のときはAボタン以外の入力を受け付けない
            resultflag=clientID;//l.389のためゲームが開始されるとresultflagに自分のclientIDを代入

/*************タックル（守備側のみ）**************************************/
            if(gClients[clientID].ADsta == 0){
                if(gClients[clientID].tackle == 2){//tackle = 0通常 1反転 2タックルに成功
                    printf("Ax,Ay=%d,%d\ngClients[cID].poi.x,gClients[cID].poi.y=%d,%d\n",Ax,Ay,gClients[cID].poi.x,gClients[cID].poi.y);
                    if(Ax != 0 || Ay != 0){
                        gClients[clientID].poi.x += Ax;
                        gClients[clientID].poi.y += Ay;  
                        Move(clientID,befx,befy,now);
                        Ax = 0;
                        Ay = 0;
                    }
                    break;
                }
                if(tflag == 0){
                    if(wiimote.keys.two){
                        switch(dirflag){
                        case up_dir:
                            gClients[clientID].poi.y = gClients[clientID].poi.y-30;
                            break;
                        case up_right_dir:
                            gClients[clientID].poi.y = gClients[clientID].poi.y-30;
                            gClients[clientID].poi.x = gClients[clientID].poi.x+30;
                        break;
                        case right_dir:
                            gClients[clientID].poi.x = gClients[clientID].poi.x+30;
                            break;
                        case right_down_dir:
                            gClients[clientID].poi.x = gClients[clientID].poi.x+30;
                            gClients[clientID].poi.y = gClients[clientID].poi.y+30;
                            break;
                        case down_dir:
                            gClients[clientID].poi.y = gClients[clientID].poi.y+30;
                            break;
                        case down_left_dir:
                            gClients[clientID].poi.y = gClients[clientID].poi.y+30;
                            gClients[clientID].poi.x = gClients[clientID].poi.x-30;
                            break;
                        case left_dir:
                            gClients[clientID].poi.x = gClients[clientID].poi.x-30;
                            break;
                        case left_up_dir:
                            gClients[clientID].poi.x = gClients[clientID].poi.x-30;
                            gClients[clientID].poi.y = gClients[clientID].poi.y-30;
                            break;
                        }
                        Move(clientID,befx,befy,now);
                        tflag++;
                        break;
                    }
                }
                else if(tflag <= 50){//50フレーム動きを止める
                    tflag++;
                    break;
                }
                else if(wiimote.keys.two != 1){//tflagが10以上かつ2が押されていない
                    tflag = 0;
                }
            }
            else if(gClients[clientID].tackle >= 2){//gClients[clientID].ADsta == 1かつ
                printf("tackle=%d tflag=%d\n\n\n",gClients[clientID].tackle,tflag);
                tflag++;
                if(tflag == 30){
                    gClients[clientID].tackle++;
                    tflag = 0;
                }
            }


            if(wiimote.keys.up || wiimote.keys.down || wiimote.keys.left || wiimote.keys.right /*&& mflag*/)
            {    
                //printf("WindowEvent\n");
                if(gClients[clientID].ADsta == 1){
                    if(wiimote.keys.one){//攻撃のダッシュ
                        game.restTime = game.restTime - 20;//ゲージを減らす
                        if(gClients[clientID].tackle != 0){//dflag = 0通常 1反転 2タックルに成功
                            a = 4 - (gClients[clientID].tackle - 1);
                            if(a == 0){
                                printf("spead a = 0\n\n\n");
//game.flag = 3;
                            }
                        }
                        else //攻撃側でtackle = 0
                            a = 4;
                    }//ダッシュしてない
                    else if(gClients[clientID].tackle != 0){//dflag = 0通常 1反転 2タックルに成功
                        a = 2 - (gClients[clientID].tackle - 1);
                        if(a == 0){
                            printf("spead a = 0\n\n\n");
//game.flag = 3;
                        }
                    }
                }
                else if(wiimote.keys.one){//守備
                    a = 4;
                }
                
                if(wiimote.keys.up){
                    if(wiimote.keys.left){
                        printf("down_left_dir");
                        gClients[clientID].poi.x = gClients[clientID].poi.x-a/2;
                        gClients[clientID].poi.y = gClients[clientID].poi.y+a/2;
                        dirflag = down_left_dir;
                    }
                    else if(wiimote.keys.right){
                        printf("left_up_dir");
                        gClients[clientID].poi.x = gClients[clientID].poi.x-a/2;
                        gClients[clientID].poi.y = gClients[clientID].poi.y-a/2;
                        dirflag = left_up_dir;
                    }
                    else{
                        printf("left_dir");
                        gClients[clientID].poi.x = gClients[clientID].poi.x-a;
                        dirflag = left_dir;
                    }
                }
                else if (wiimote.keys.down){
                    if(wiimote.keys.left){
                        printf("right_down_dir");
                        gClients[clientID].poi.x = gClients[clientID].poi.x+a/2;
                        gClients[clientID].poi.y = gClients[clientID].poi.y+a/2;
                        dirflag = right_down_dir;
                    }
                    else if(wiimote.keys.right){
                        printf("up_right_dir");
                        gClients[clientID].poi.x = gClients[clientID].poi.x+a/2;
                        gClients[clientID].poi.y = gClients[clientID].poi.y-a/2;
                        dirflag = up_right_dir;
                    }
                    else{
                        printf("right_dir");
                        gClients[clientID].poi.x = gClients[clientID].poi.x+a;
                        dirflag = right_dir;
                    }
                }
                else if(wiimote.keys.left){
                    /*  if(wiimote.keys.up){
                        printf("down_left_dir");
                        gClients[clientID].poi.y = gClients[clientID].poi.y+a/2;
                        gClients[clientID].poi.x = gClients[clientID].poi.x-a/2;
                        dirflag = down_left_dir;
                    }
                    else if(wiimote.keys.down){
                        printf("right_down_dir");
                        gClients[clientID].poi.y = gClients[clientID].poi.y+a/2;
                        gClients[clientID].poi.x = gClients[clientID].poi.x+a/2;
                        dirflag = right_down_dir;
                    }
                    else{*/
                        printf("down_dir");
                        gClients[clientID].poi.y = gClients[clientID].poi.y+a;
                        dirflag = down_dir;
                        //    }
                }
                else if(wiimote.keys.right){
                    /*  if(wiimote.keys.up){
                        printf("left_up_dir");
                        gClients[clientID].poi.y = gClients[clientID].poi.y-a/2;
                        gClients[clientID].poi.x = gClients[clientID].poi.x-a/2;
                        dirflag = left_up_dir;
                    }
                    else if(wiimote.keys.down){
                        printf("up_right_dir");
                        gClients[clientID].poi.y = gClients[clientID].poi.y-a/2;
                        gClients[clientID].poi.x = gClients[clientID].poi.x+a/2;
                        dirflag = up_right_dir;
                    }
                    else{*/
                        printf("up_dir");
                        gClients[clientID].poi.y = gClients[clientID].poi.y-a;
                        dirflag = up_dir;
                        //  }
                }
                mflag = 0;
                Move(clientID,befx,befy,now);
                break;
            }
        }
/**********************************************************************************
game.flag: 0メイン画面 1ゲーム画面　2ゲームループ 3各ピリオド終了　4カバディ終了
**********************************************************************************/
        else if(game.flag == 0){//メイン画面
            if(wiimote.keys.a)
            {
                /*     char comment[64];
                SDL_Rect dst_rect2 = { 350, 350 };
                SDL_Surface *gMessage_comment;
                
                SDL_FillRect(buffer,NULL,0xffffffff); //背景を白にする
                sprintf(comment,"待機中");
                gMessage_comment = TTF_RenderUTF8_Blended(font, comment, colB);
                SDL_Rect src_rect2 = { 0, 0, gMessage_comment->w,gMessage_comment->h };
                SDL_BlitSurface(gMessage_comment, &src_rect2, buffer, &dst_rect2);
                
                SDL_BlitSurface(buffer, NULL, gMainWindow, NULL);
                SDL_Flip(gMainWindow);
                */
                
                sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d\0",RESTART,clientID,0,0,0,0,0);
                SendData(data);
            }
        }

        else if(game.flag == 3){//各ピリオド終了
            // if(gClients[clientID].restart==0){
            if(wiimote.keys.plus)//プラスキー
            {
                if(continueflag==0)
                {
                    continueflag=1;
                    resultflag++;
                    WinDisplay(resultflag);
                }
            }
            else if(continueflag==1)
            {
                continueflag=0;
            }
            if(wiimote.keys.minus)//マイナスキー
            {
                if(continueflag==0)
                {
                    continueflag=2;
                    resultflag--;
                    WinDisplay(resultflag);
                }
            }
            else if(continueflag==2)
            {
                    continueflag=0;
//<<<<<<< HEAD
            }
            if(wiimote.keys.a)
            {
                char comment[64];
                SDL_Rect dst_rect2 = { 350, 350 };
                SDL_Surface *gMessage_comment;
                
                SDL_FillRect(buffer,NULL,0xffffffff); /*背景を白にする*/
                sprintf(comment,"待機中");
                gMessage_comment = TTF_RenderUTF8_Blended(font, comment, colB);
                SDL_Rect src_rect2 = { 0, 0, gMessage_comment->w,gMessage_comment->h };
                SDL_BlitSurface(gMessage_comment, &src_rect2, buffer, &dst_rect2);
                
                SDL_BlitSurface(buffer, NULL, gMainWindow, NULL);
                SDL_Flip(gMainWindow);
                
                sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d\0",RESTART,clientID,0,0,0,0,0);
                SendData(data);
            }
//=======
//>>>>>>> color
        }
            
            /*Aボタン（リスタート）*/
//            if(wiimote.keys.a)
//            {
//                char comment[64];
//                SDL_Rect dst_rect2 = { 350, 350 };
//                SDL_Surface *gMessage_comment;
                
//                SDL_FillRect(buffer,NULL,0xffffffff); /*背景を白にする*/
//                              sprintf(comment,"待機中");
//                gMessage_comment = TTF_RenderUTF8_Blended(font, comment, colB);
//                SDL_Rect src_rect2 = { 0, 0, gMessage_comment->w,gMessage_comment->h };
//                SDL_BlitSurface(gMessage_comment, &src_rect2, buffer, &dst_rect2);
                
//                SDL_BlitSurface(buffer, NULL, gMainWindow, NULL);
//                SDL_Flip(gMainWindow);
                
//                sprintf(data,"kabaddi,%d,%d,%d,%d,%d\0",RESTART,clientID,0,0,0);
//                SendData(data);
//                }
//        }

        else if(game.flag == 4){
                
            /*Aボタン（リスタート）*/
            if(wiimote.keys.minus)
            {
                /*
                  char comment[64];
                SDL_Rect dst_rect2 = { 350, 350 };
                SDL_Surface *gMessage_comment;
                
                SDL_FillRect(buffer,NULL,0xffffffff); //背景を白にする
                sprintf(comment,"待機中");
                gMessage_comment = TTF_RenderUTF8_Blended(font, comment, colB);
                SDL_Rect src_rect2 = { 0, 0, gMessage_comment->w,gMessage_comment->h };
                SDL_BlitSurface(gMessage_comment, &src_rect2, buffer, &dst_rect2);
                
                SDL_BlitSurface(buffer, NULL, gMainWindow, NULL);
                SDL_Flip(gMainWindow);
                */                

                sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d\0",RESTART,clientID,0,0,0,0,0);
                SendData(data);
            }   
        }
        //   }
        break;
    }
}

/*****
static
*****/

void DrawChara(int n,int cnum)
{

    //printf("crect = %d  %d\n",chara_rect[clientID].x,chara_rect[clientID].y);
    int i,j,tmp;
    int num=3;

    int s[]={0,1,2,3,4,5,6,7};

    SDL_FillRect(buffer,NULL,0xffffffff);
    DisplayStatus();

    lineColor(buffer, 800, 0, 800, 600,0x000000ff);

    //printf("cnum = %d\n" ,cnum);

    for(i=0;i<cnum;++i){
        for(j=i+1;j<cnum;++j){
            if(gClients[s[i]].poi.y > gClients[s[j]].poi.y){
                tmp = s[i];
                s[i]=s[j];
                s[j]=tmp;
            }
        }
    }

    for(i=0;i<cnum;i++){
        j=s[i];
        //printf("s[%d]=%d\n",i,j);
        //printf("ID%d = %d  %d\n",i,gClients[i].poi.x,gClients[i].poi.y);
        SDL_BlitSurface(gCharaImage,&chara_rect[j],buffer,&gClients[j].poi);
        //SDL_FillRect(buffer,&gClients[i].poi,color[0]);
        //文字表示
        PNAME_rrect[j].x = gClients[j].poi.x;
        PNAME_rrect[j].y = gClients[j].poi.y - 5;
        SDL_BlitSurface(PNAME[j], &PNAME_srect[j], buffer,&PNAME_rrect[j]);    
    }
    //   for(i=0;i<num;i++){
        //    printf("%d %d\n",gClients[i].poi.x,gClients[i].poi.y);
    //  }

    //printf("%d\n",n);
    //Judge(n,cnum);

                       /*始点x座標，始点y座標，終点x座標，終点y座標，色*/
    /*for(i=0;i<cnum;i++){
        if(gClients[i].ADsta==1){
            rectangleColor(buffer,gClients[i].poi.x-20,gClients[i].poi.y-20,gClients[i].poi.x+50,gClients[i].poi.y+50,0xaaaaaaff);
        }
        SDL_FillRect(buffer,&gClients[i].poi,color[gClients[i].color]);
        
        
        }*/



    
    
    SDL_BlitSurface(buffer, NULL, gMainWindow, &brect);
    
    SDL_Flip(gMainWindow);
   
}

void WinDisplay(int ID)//引数clientID,WindowEventからの場合はresultflag
{
    int i;
    char   status[64];

    SDL_Rect dst_rect2 = { 350, 350 };
    SDL_Surface *gMessage_score;

    i= ID;

    if(resultflag+1>cnum)
    {
        resultflag=0;
        i = 0;
    }
    else if(resultflag<0)
    {
        resultflag=cnum-1;
        i = cnum-1;
    }

    SDL_FillRect(bufmain,NULL,0xffffffff); /*背景を白にする*/
    sprintf(status,"%s score:%dpt",gClients[i].name,gClients[i].score);
    
    gMessage_score = TTF_RenderUTF8_Blended(font, status, colB);
    SDL_Rect src_rect2 = { 0, 0, gMessage_score->w,gMessage_score->h };
    SDL_BlitSurface(gMessage_score, &src_rect2, bufmain, &dst_rect2);
    
    SDL_BlitSurface(bufmain, NULL, gMainWindow, NULL);
    SDL_Flip(gMainWindow);
}



void DisplayStatus(void)//自分のスタミナの描写
{
    //game.restTime--;
    //char   status[64];
    //  SDL_Surface *mes;
    //SDL_Rect dst_rect = {0,0};//転送先
    //SDL_Rect src_rect = {0,0,0,0};//転送元
    //printf("callback\n");
    /*
    if(game.restTime > 0){
        sprintf(status,"残り%d秒 score:%dpt",game.restTime/10,gClients[clientID].score);
        printf("%s\n",status);
    }
    else
        sprintf(status,"タイムアップ");

    mes = TTF_RenderUTF8_Blended(font2, status, colB);
    //Src_rect.w = mes->w;
    //Src_rect.h = mes->h;//転送元

    // 背景を白にする 
    SDL_FillRect(scbuf,NULL,0xffffffff);
    SDL_BlitSurface(mes, NULL, scbuf, NULL);
    SDL_BlitSurface(scbuf, NULL, gMainWindow, NULL);
    */
    
    STrect.x = (game.restTime/100)*3;
    STrect.w = 600 - (game.restTime/100)*3;
//背景を水色に
    if(game.restTime > 12000)
        SDL_FillRect(stbar,NULL,stcol[0]);
    else if(game.restTime > 5000)
        SDL_FillRect(stbar,NULL,stcol[1]);
    else 
        SDL_FillRect(stbar,NULL,stcol[2]);


    SDL_FillRect(stbar,&STrect,0xffffffff);
   
    SDL_BlitSurface(stbar, NULL, gMainWindow, &srect);
    //SDL_Flip(gMainWindow);

}
