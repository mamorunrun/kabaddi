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

int color[MAX_CLIENTS] = {0x000000ff,0x00ff000};

/*****************************************************************
関数名	: InitWindows
機能	: メインウインドウの表示，設定を行う
引数	: int	clientID		: クライアント番号
		  int	num				: 全クライアント数
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
int InitWindows(int clientID,char name[][MAX_NAME_SIZE])
{
    cID = clientID;
	int i;
	char *s,title[10];

        /* 引き数チェック */
        assert(0<num && cnum<=MAX_CLIENTS);
	
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

	/* ウインドウのタイトルをセット */
	sprintf(title,"Kabaddi[%d]",clientID);
	SDL_WM_SetCaption(title,NULL);

	/* 背景を白にする */
	SDL_FillRect(buffer,NULL,0xffffffff);

        for(i=0;i<cnum;i++){
            gClients[i].poi.x=100*i + 10;
            gClients[i].poi.y=10;
            gClients[i].poi.w=30;
            gClients[i].poi.h=30;
            gClients[i].ADsta = i;
            SDL_FillRect(buffer,&gClients[i].poi, color[gClients[i].ADsta]);
        }
        SDL_BlitSurface(buffer, NULL, gMainWindow, NULL);
	SDL_Flip(gMainWindow);
	
        //printf("%d\n",color);

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
    for(i=0;i<cnum;i++){
        SDL_FillRect(buffer,&gClients[i].poi,color[gClients[i].ADsta]);
    }
    SDL_BlitSurface(buffer, NULL, gMainWindow, NULL);
    
    SDL_Flip(gMainWindow);
    dflag = 0;
}
