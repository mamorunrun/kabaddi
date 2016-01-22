/*****************************************************************
ファイル名	: client_win.c
機能		: クライアントのユーザーインターフェース処理
*****************************************************************/

#include<SDL/SDL.h>
#include<SDL/SDL_ttf.h>
#include<SDL/SDL_gfxPrimitives.h>
#include"common.h"
#include"client_func.h"

static SDL_Surface *gMainWindow;
static SDL_Surface *buffer;
static SDL_Surface *scbuf;//得点を表すバッファ
static SDL_Surface *stbar;//スタミナを表すバッファ
SDL_Rect STrect = {0, 0, 0, 50};//スタミナゲージのため
SDL_Rect srect = {400,0};//stbarの領域
SDL_Rect brect = {0, 51};//bufferからの領域
static int cID;
CLIENT gClients[MAX_CLIENTS];

void DisplayStatus(void);

static int tflag;//タックルのフラグ

int dflag;//mainとのグローバル変数,動いたことの検知
int dirflag;//方向を表す

int color[5] = {0x0000ffff,0xff0000ff,0x00ff00ff,0xff00ffff,0x00ff00ff};//2と4はcommand.c内l.65のため同じ
int stcol[3] = {0x0000ff,0xffff00,0xff0000};//青,黄色,赤
SDL_Color colB = {0,0,0};//黒色（文字）
static TTF_Font* font;	// TrueTypeフォントデータへのポインタ
static TTF_Font* font2;
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

    /* フォントの初期化 */
    TTF_Init();

    font = TTF_OpenFont("kochi-gothic-subst.ttf",48); // フォントの設定kochi-gothic-substフォントを48ポイントで使用（読み込み）
    font2 = TTF_OpenFont("kochi-gothic-subst.ttf",24);

    /* ウインドウのタイトルをセット */
    sprintf(title,"Kabaddi[%d]",clientID);
    SDL_WM_SetCaption(title,NULL);
    
    /* 背景を白にする */
    SDL_FillRect(buffer,NULL,0xffffffff);

    gMessage_title = TTF_RenderUTF8_Blended(font, "白熱カバッディ", /*0x191970ff*/colB);
    SDL_Rect src_rect = { 0, 0, gMessage_title->w,gMessage_title->h };
    gMessage_req = TTF_RenderUTF8_Blended(font2, "スペースキーを押すんだ！", /*0x191970ff*/colB);
    SDL_Rect src_rect2 = { 0, 0, gMessage_req->w,gMessage_req->h  };

    SDL_BlitSurface(gMessage_title, &src_rect, buffer, &dst_rect);
    SDL_BlitSurface(gMessage_req, &src_rect2, buffer, &dst_rect2);

    SDL_BlitSurface(buffer, NULL, gMainWindow, &brect);
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
        SDL_FillRect(buffer,NULL,0xffffffff);

        gMessage_chotomate = TTF_RenderUTF8_Blended(font, "ちょっと待ってくれ！", /*0x000000ff*/colB);
    SDL_Rect src_rect3 = { 0, 0, gMessage_chotomate->w,gMessage_chotomate->h  }; 
        SDL_BlitSurface(gMessage_chotomate, &src_rect3, buffer, &dst_rect3);   

    SDL_BlitSurface(buffer, NULL, gMainWindow, &brect);
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


        game.restTime = 30000;/*残り30000ミリ（30）秒*/
        lineColor(buffer, 800, 0, 800, 600,0x000000ff);
        /*始点x座標，始点y座標，終点x座標，終点y座標，色*/



	/* 背景を白にする */
	SDL_FillRect(buffer,NULL,0xffffffff);
       
        int i,x,y;

        for(i=0;i<cnum;i++){
            if(i == (loop % cnum)){
                gClients[i].poi.x=700;
                gClients[i].poi.y=250;
                gClients[i].poi.w=30;
                gClients[i].poi.h=30;
                gClients[i].ADsta = 1;/*最初は攻撃*/
                gClients[i].color=1;
            }
            else{
                gClients[i].poi.x=200;
                gClients[i].poi.y=100 + i*200;
                gClients[i].poi.w=30;
                gClients[i].poi.h=30;
                gClients[i].ADsta = 0;/*最後二人は守備*/
                gClients[i].color=0;
            }

            if(gClients[i].ADsta==1){
                rectangleColor(buffer,gClients[i].poi.x-20,gClients[i].poi.y-20,gClients[i].poi.x+50,gClients[i].poi.y+50,0x000000ff);
            }

            printf("%d,%d,%d\n",i,gClients[i].poi.x,gClients[i].poi.y);
            gClients[i].Bflag = 0;
            SDL_FillRect(buffer,&gClients[i].poi, color[gClients[i].ADsta]);

/***************************************************************************
            四角の上に文字を出力 SDL_BlitSurfaceの活用
            sprintf(Pname[i],"%d:%s",i,name);
            
            PNAME = TTF_RenderUTF8_Blended(font, Pname[i], );
            SDL_Rect src_rect = {0,0 }
            
            SDL_BlitSurface(PNAME, &rect, SDL_GetVideoSurface(), &scr_rect);
          
            
*****************************************************************************/
        }
        printf("loop=%d\n",loop);

/*******************************************************************************
得点の描写
 ****************************************************************************/
        char   status[64];

        sprintf(status,"score:%dpt",gClients[clientID].score);
        printf("%s\n",status);
    
        mes = TTF_RenderUTF8_Blended(font2, status, colB);
  
    // 背景を白にする 
        SDL_FillRect(scbuf,NULL,0xffffffff);
        SDL_BlitSurface(mes, NULL, scbuf, NULL);
        SDL_BlitSurface(scbuf, NULL, gMainWindow, NULL);
        
        SDL_BlitSurface(buffer, NULL, gMainWindow, &brect);
	SDL_Flip(gMainWindow);
        

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

void WindowEvent(int clientID)
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

        if(game.flag == 0){//ゲームフラグが1のときはAボタン以外の入力を受け付けない
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
                    Move(clientID,befx,befy);
                    tflag++;
                    break;
                }
            }

            else if(tflag <= 8){
                //8フレーム動きを止める
                tflag++;
                break;
            }

            else if(tflag == 9){
                switch(dirflag){
                case up_dir:
                    gClients[clientID].poi.y = gClients[clientID].poi.y+30;
                    break;
                case up_right_dir:
                    gClients[clientID].poi.y = gClients[clientID].poi.y+30;
                    gClients[clientID].poi.x = gClients[clientID].poi.x-30;
                    break;
                case right_dir:
                    gClients[clientID].poi.x = gClients[clientID].poi.x-30;
                    break;
                case right_down_dir:
                    gClients[clientID].poi.x = gClients[clientID].poi.x-30;
                    gClients[clientID].poi.y = gClients[clientID].poi.y-30;
                    break;
                case down_dir:
                    gClients[clientID].poi.y = gClients[clientID].poi.y-30;
                    break;
                case down_left_dir:
                    gClients[clientID].poi.y = gClients[clientID].poi.y-30;
                    gClients[clientID].poi.x = gClients[clientID].poi.x+30;
                    break;
                case left_dir:
                    gClients[clientID].poi.x = gClients[clientID].poi.x+30;
                    break;
                case left_up_dir:
                    gClients[clientID].poi.x = gClients[clientID].poi.x+30;
                    gClients[clientID].poi.y = gClients[clientID].poi.y+30;
                    break;
                }
                //Move(clientID,befx,befy);
                tflag++;
                break;
            }
            else if(wiimote.keys.two != 1)//tflagが10以上かつ2が押されていない
            {
                tflag = 0;
            }
            
            if(wiimote.keys.one){
                if(gClients[clientID].ADsta == 1)
                    game.restTime = game.restTime - 50;//ゲージを減らす
                
                a = 4;
            }
            
            if(wiimote.keys.up || wiimote.keys.down || wiimote.keys.left || wiimote.keys.right /*&& mflag*/)
            {    
                printf("WindowEvent\n");
                
                if(wiimote.keys.up){
                    gClients[clientID].poi.x = gClients[clientID].poi.x-a;
                    dirflag = left_dir;
                }
                else if (wiimote.keys.down){
                    gClients[clientID].poi.x = gClients[clientID].poi.x+a;
                    dirflag = right_dir;
                }
                if(wiimote.keys.left){
                    gClients[clientID].poi.y = gClients[clientID].poi.y+a;
                    dirflag = down_dir;
                }
                else if(wiimote.keys.right){
                    gClients[clientID].poi.y = gClients[clientID].poi.y-a;
                    dirflag = up_dir;
                }
                mflag = 0;
                Move(clientID,befx,befy);
                break;
            }
        }

        if(game.flag == 1){
            if(gClients[clientID].restart==0){
                if(wiimote.keys.a)
                {
                    sprintf(data,"kabaddi,%d,%d,%d,%d,%d\0",RESTART,clientID,0,0,0);
                    SendData(data);
                }
            }
        }
        break;
    }
}

/*****
static
*****/

void DrawChara(int n,int cnum)
{

    int i;
    int num=3;

    //   for(i=0;i<num;i++){
        //    printf("%d %d\n",gClients[i].poi.x,gClients[i].poi.y);
    //  }

    //printf("%d\n",n);
    //Judge(n,cnum);
    SDL_FillRect(buffer,NULL,0xffffffff);
    DisplayStatus();

    lineColor(buffer, 800, 0, 800, 600,0x000000ff);
                       /*始点x座標，始点y座標，終点x座標，終点y座標，色*/
    for(i=0;i<cnum;i++){
        if(gClients[i].ADsta==1){
            rectangleColor(buffer,gClients[i].poi.x-20,gClients[i].poi.y-20,gClients[i].poi.x+50,gClients[i].poi.y+50,0xaaaaaaff);
        }
        SDL_FillRect(buffer,&gClients[i].poi,color[gClients[i].color]);
        
        
    }
    
    
    SDL_BlitSurface(buffer, NULL, gMainWindow, &brect);
    
    SDL_Flip(gMainWindow);
    dflag = 0;
}

void WinDisplay(void)
{
    int i;
    char   status[64];

    //   SDL_Rect dst_rect = { 350, 250 };
    SDL_Rect dst_rect2 = { 350, 350 };
//    SDL_Surface *gMessage_win;
    SDL_Surface *gMessage_score;
    
    for(i=0;i<cnum;i++){
        if(gClients[i].ADsta == 1){
            printf("win\n");
            SDL_FillRect(buffer,NULL,0xffffffff); /*背景を白にする*/
            
//            gMessage_win = TTF_RenderUTF8_Blended(font, "Win", /*0x191970ff*/colB);
//            SDL_Rect src_rect = { 0, 0, gMessage_win->w,gMessage_win->h };
//            SDL_BlitSurface(gMessage_win, &src_rect, buffer, &dst_rect);
            sprintf(status,"score:%dpt",gClients[i].score);

            gMessage_score = TTF_RenderUTF8_Blended(font, status, /*0x191970ff*/colB);
            SDL_Rect src_rect2 = { 0, 0, gMessage_score->w,gMessage_score->h };
            SDL_BlitSurface(gMessage_score, &src_rect2, buffer, &dst_rect2);
            
            SDL_BlitSurface(buffer, NULL, gMainWindow, NULL);
            SDL_Flip(gMainWindow);
        }
    }

}


void DisplayStatus(void)//自分のスタミナの描写
{
    //game.restTime--;
    //char   status[64];
    //  SDL_Surface *mes;
    //SDL_Rect dst_rect = {0,0};//転送先
    //SDL_Rect src_rect = {0,0,0,0};//転送元
    printf("callback\n");
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
    
    STrect.x = (game.restTime/100)*2;
    STrect.w = 600 - (game.restTime/100)*2;
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
