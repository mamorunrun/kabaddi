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
CLIENT gClients[MAX_CLIENTS];

/*****************************************************************
関数名	: InitWindows
機能	: メインウインドウの表示，設定を行う
引数	: int	clientID		: クライアント番号
		  int	num				: 全クライアント数
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE])
{
	int i;
	char *s,title[10];

        /* 引き数チェック */
        assert(0<num && num<=MAX_CLIENTS);
	
	/* SDLの初期化 */
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("failed to initialize SDL.\n");
		return -1;
	}
	
	/* メインのウインドウを作成する */
	if((gMainWindow = SDL_SetVideoMode(400,240, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		return -1;
	}
        if((buffer = SDL_CreateRGBSurface(SDL_SWSURFACE,400,240,32,0,0,0,0))==NULL){
            printf("failed to initialize videomode.\n");
            exit(-1);
        }

	/* ウインドウのタイトルをセット */
	sprintf(title,"Kabaddi[%d]",clientID);
	SDL_WM_SetCaption(title,NULL);

	
	/* 背景を白にする */
	SDL_FillRect(buffer,NULL,0xffffffff);

        for(i=0;i<num;i++){
            gClients[i].poi.x=10*i+30;
            gClients[i].poi.y=10;
            gClients[i].poi.w=30;
            gClients[i].poi.h=30;

            SDL_FillRect(buffer,&gClients[i].poi,0x000000ff);
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

void WindowEvent(int num)
{
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
        if(wiimote.keys.up){
            printf("up\n");
            SendRightCommand();
        }
        if (wiimote.keys.down){
            SendLeftCommand();
        }
        if(wiimote.keys.left){
            SendUpCommand();
        }
        if(wiimote.keys.right){
            SendDownCommand();
        }
        
        break;
       
    }
    
}

/*****
static
*****/

void DrawChara(int n, int x, int y)
{

    int i;
    int num=2;

    gClients[n].poi.x=x;
    gClients[n].poi.y=y;

    SDL_FillRect(buffer,NULL,0xffffffff);
    for(i=0;i<num;i++){
        SDL_FillRect(buffer,&gClients[i].poi,0x000000ff);
    }
    SDL_BlitSurface(buffer, NULL, gMainWindow, NULL);
    
    SDL_Flip(gMainWindow);
    
}

