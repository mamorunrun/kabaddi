/*****************************************************************
ファイル名	: client_win.c
機能		: クライアントのユーザーインターフェース処理
*****************************************************************/

#include<SDL/SDL.h>
#include<SDL/SDL_gfxPrimitives.h>
#include"common.h"
#include"client_func.h"

static SDL_Surface *gMainWindow;
static SDL_Surface *buffer;
static int cID;
CLIENT gClients[MAX_CLIENTS];


static int tflag;//タックルのフラグ

int dflag;//mainとのグローバル変数,動いたことの検知
int dirflag;//方向を表す

int color[2] = {0x000000ff,0xff000000};

static TTF_Font* font;	// TrueTypeフォントデータへのポインタ
static TTF_Font* font2;


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
    int end = 0;

    SDL_Event event;
    SDL_Rect dst_rect = { 100, 200 };
    SDL_Rect dst_rect2 = { 100, 400 };
    SDL_Rect src_rect = { 0, 0, gMessage->w, gMessage->h };
    SDL_Rect src_rect2 = { 0, 0, gMessage->w, gMessage->h };
//    SDL_Rect src_rect3 = { 100, 400, gMessage->w, gMessage->h };
    SDL_Surface gMessage_title;
    SDL_Surface gMessage_req;
    SDL_Surface gMessage_chotomate;
    
    /* SDLの初期化 */
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("failed to initialize SDL.\n");
        return -1;
    }
    
    /* メインのウインドウを作成する */
    if((gMainWindow = SDL_SetVideoMode(1000,600, 32, SDL_SWSURFACE)) == NULL) {
        printf("failed to initialize videomode.\n");
        return -1;
    }
    if((buffer = SDL_CreateRGBSurface(SDL_SWSURFACE,1000,600,32,0,0,0,0))==NULL){
        printf("failed to initialize videomode.\n");
        exit(-1);
    }

    /* フォントの初期化 */
    TTF_Init();

    font = TTF_OpenFont("kochi-gothic-subst.ttf",32); // フォントの設定kochi-gothic-substフォントを24ポイントで使用（読み込み）
    font2 = TTF_OpenFont("kochi-gothic-subst.ttf",24); // フォントの設定kochi-gothic-substフォントを24ポイントで使用（読み込み）
    
    /* ウインドウのタイトルをセット */
    sprintf(title,"Kabaddi[%d]",clientID);
    SDL_WM_SetCaption(title,NULL);
    
    /* 背景を白にする */
    SDL_FillRect(buffer,NULL,0xffffffff);

    gMessage_title = TTF_RenderUTF8_Blended(font, "白熱カバッディ", 0x191970ff);
    gMessage_req = TTF_RenderUTF8_Blended(font2, "Enterキーを押すんだぜ！", 0x191970ff);    

    SDL_BlitSurface(gMessage_title, &src_rect, buffer, &dst_rect);
    SDL_BlitSurface(gMessage_req, &src_rect2, buffer, &dst_rect2);

    SDL_BlitSurface(buffer, NULL, gMainWindow, NULL);
    SDL_Flip(gMainWindow);

    while(end == 0){
        if(SDL_PollEvent(&event)){// イベント取得ができた場合イベントにより処理を分岐
            switch(event.type){
            case SDLK_KEYDOWN:
                if(event.key.keysym.sym == SDLK_RETURN)  
                    end = 1;
                break;

            default:
                break;
            }
        }

        /* 背景を白にする */
        SDL_FillRect(buffer,NULL,0xffffffff);

        gMessage_chotomate = TTF_RenderUTF8_Blended(font3, "ちょっと待ってちょっと待って...", 0x000000ff);

        SDL_BlitSurface(gMessage_chotomate, &src_rect2, buffer, &dst_rect2);

    SDL_BlitSurface(buffer, NULL, gMainWindow, NULL);
    SDL_Flip(gMainWindow);
    
    return 0;
}
/*****************************************************************
関数名	: InitWindows
機能	: ゲームウインドウの表示，設定を行う
引数	: int	clientID		: クライアント番号
		
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
int GameWindows(int clientID,char name[][MAX_NAME_SIZE])
{
    
     /* 引き数チェック */
        assert(0<num && cnum<=MAX_CLIENTS);
	
/*　　　文字関係
        char Pname[cnum][MAX_NAME_SIZE+2];
        SDL_Surface *PNAME[cnum];
*/      

        lineColor(buffer, 600, 0, 600, 600,0x000000ff);
        /*始点x座標，始点y座標，終点x座標，終点y座標，色*/



	/* 背景を白にする */
	SDL_FillRect(buffer,NULL,0xffffffff);
       
        for(i=0;i<cnum;i++){
            if(i < 2){
                gClients[i].poi.x=200;
                gClients[i].poi.y=100 + i*300;
                gClients[i].poi.w=30;
                gClients[i].poi.h=30;
                gClients[i].ADsta = 0;/*最初二人は守備*/
            }
            else{
                gClients[i].poi.x=700;
                gClients[i].poi.y=250;
                gClients[i].poi.w=30;
                gClients[i].poi.h=30;
                gClients[i].ADsta = 1;/*最後は攻撃*/
            }
            SDL_FillRect(buffer,&gClients[i].poi, color[gClients[i].ADsta]);
            
/***************************************************************************
            四角の上に文字を出力 SDL_BlitSurfaceの活用
            sprintf(Pname[i],"%d:%s",i,name);
            
            PNAME = TTF_RenderUTF8_Blended(font, Pname[i], );
            SDL_Rect src_rect = {0,0 }
            
            SDL_BlitSurface(PNAME, &rect, SDL_GetVideoSurface(), &scr_rect);
          
            
*****************************************************************************/
        }
        SDL_BlitSurface(buffer, NULL, gMainWindow, NULL);
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
	SDL_Quit();
}

void WindowEvent(int clientID)
{
    int a = 2;
    int mflag = 1;
    int befx,befy;

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
            SendEndCommand();
        }
        if(tflag == 0)
        {
            if(wiimote.keys.two)
            {
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
        else if(tflag == 1)
        {
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
            Move(clientID,befx,befy);
            tflag++;
            break;
        }
        else if(wiimote.keys.two != 1)
        {
            tflag = 0;
        }
        
        if(wiimote.keys.one){
            a = 4;
        }
        if(wiimote.keys.up || wiimote.keys.down || wiimote.keys.left || wiimote.keys.right && mflag)
        {
            if(wiimote.keys.up){
                gClients[clientID].poi.x = gClients[clientID].poi.x-a;
                //Move(clientID);
                
                dirflag = left_dir;
            }
            else if (wiimote.keys.down){
                gClients[clientID].poi.x = gClients[clientID].poi.x+a;
                //Move(clientID);
                
                dirflag = right_dir;
            }
            if(wiimote.keys.left){
                gClients[clientID].poi.y = gClients[clientID].poi.y+a;
                //Move(clientID);
                
                dirflag = down_dir;
            }
            else if(wiimote.keys.right){
                gClients[clientID].poi.y = gClients[clientID].poi.y-a;
                //Move(clientID);
                
                dirflag = up_dir;
            }
            Move(clientID,befx,befy);
            mflag = 0;
        }
        break;
    }
    //DrawChara(clientID);
    
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
    lineColor(buffer, 700, 0, 700, 600,0x000000ff);
                       /*始点x座標，始点y座標，終点x座標，終点y座標，色*/
    for(i=0;i<cnum;i++){
        SDL_FillRect(buffer,&gClients[i].poi,color[gClients[i].ADsta]);
    }
    SDL_BlitSurface(buffer, NULL, gMainWindow, NULL);
    
    SDL_Flip(gMainWindow);
    dflag = 0;
}
