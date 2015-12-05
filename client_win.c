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
static void Move(int clientID);
CLIENT gClients[MAX_CLIENTS];


static int tflag;//タックルのフラグ

int dflag;//mainとのグローバル変数,動いたことの検知
int dirflag;//方向を表す

//static int color;

/*****************************************************************
関数名	: InitWindows
機能	: メインウインドウの表示，設定を行う
引数	: int	clientID		: クライアント番号
		  int	num				: 全クライアント数
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE])
{
    cID = clientID;
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

//	color = 0x000000ff;
	/* 背景を白にする */
	SDL_FillRect(buffer,NULL,0xffffffff);

        for(i=0;i<num;i++){
            gClients[i].poi.x=10*i+30;
            gClients[i].poi.y=10;
            gClients[i].poi.w=30;
            gClients[i].poi.h=30;

            SDL_FillRect(buffer,&gClients[i].poi, /*color*/0x000000ff);
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
                case right_dir:
                    gClients[clientID].poi.x = gClients[clientID].poi.x+30;
                    break;
                case down_dir:
                    gClients[clientID].poi.y = gClients[clientID].poi.y+30;
                    break;
                case left_dir:
                    gClients[clientID].poi.x = gClients[clientID].poi.x-30;
                    break;
                }
                Move(clientID);
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
            case right_dir:
                gClients[clientID].poi.x = gClients[clientID].poi.x-30;
                    break;
            case down_dir:
                gClients[clientID].poi.y = gClients[clientID].poi.y-30;
                    break;
            case left_dir:
                gClients[clientID].poi.x = gClients[clientID].poi.x+30;
                    break;
            }
            Move(clientID);
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
        if(wiimote.keys.up){
            gClients[clientID].poi.x = gClients[clientID].poi.x-a;
            Move(clientID);

            dirflag = left_dir;
        }
        else if (wiimote.keys.down){
            gClients[clientID].poi.x = gClients[clientID].poi.x+a;
            Move(clientID);

            dirflag = right_dir;
        }
        else if(wiimote.keys.left){
            gClients[clientID].poi.y = gClients[clientID].poi.y+a;
            Move(clientID);

            dirflag = down_dir;
        }
        else if(wiimote.keys.right){
            gClients[clientID].poi.y = gClients[clientID].poi.y-a;
            Move(clientID);

            dirflag = up_dir;
        }
        break;
    }
    DrawChara(clientID);
    
}

/*****
static
*****/

void DrawChara(int n)
{
    int i;
    int num=2;

    //   for(i=0;i<num;i++){
        //    printf("%d %d\n",gClients[i].poi.x,gClients[i].poi.y);
    //  }

    SDL_FillRect(buffer,NULL,0xffffffff);
    for(i=0;i<num;i++){
        SDL_FillRect(buffer,&gClients[i].poi,/*color*/0x000000ff);
    }
    SDL_BlitSurface(buffer, NULL, gMainWindow, NULL);
    
    SDL_Flip(gMainWindow);
    dflag = 0;
}

void UpdatePos(int n,int x,int y)
{
    if(cID == n)
        return;

    gClients[n].poi.x=x;
    gClients[n].poi.y=y;
}

void Move(int clientID)
{
    char	data[MAX_DATA];
    int			dataSize = 0;

    dflag = 1;

    if(gClients[clientID].poi.x <= 0)
        gClients[clientID].poi.x = 0;
    else if(gClients[clientID].poi.x + 30 >= 1000)
        gClients[clientID].poi.x = 1000 - 30;
    if(gClients[clientID].poi.y <= 0)
        gClients[clientID].poi.y = 0;
    else if(gClients[clientID].poi.y + 30 >= 600)
        gClients[clientID].poi.y = 600 -30;

    sprintf(data,"kabaddi,%d,%d,%d,%d\0",CDRAW,clientID,gClients[clientID].poi.x,gClients[clientID].poi.y);

    printf("%s\n",data);
    //strcat(data,gClients[clientID].poi.x);
    //strcat(data,",");
    //strcat(data,gClients[clientID].poi.y);
    //strcat(data,"\0");

    // SetCharData2DataBlock(data,POS_COMMAND,&dataSize);
    //SetIntData2DataBlock(data,gClients[clientID].poi.x,&dataSize);
    //SetIntData2DataBlock(data,gClients[clientID].poi.y,&dataSize);
    SendData(data/*, dataSize*/);
}
/*
int Judge(int clientID){

    int i;
    int n = 2;//グローバル変数で守備人数を設定
    if(gClients[clientID].ADsta == 0)//自分が守備
        for(i=0;i>=n;i++){//守備人数分
            if(i != clientID){
                if(gClients[i].ADsta == 1){
                    if(gClients[i].poi.x - gClients[clientID].poi.x <= 40 && gClients[clientID].poi.x - gClients[i].poi.x >= -40){
                        if(gClients[i].poi.y - gClients[clientID].poi.y <= 40 && gClients[clientID].poi.y - gClients[i].poi.y >= -40){
                            color = 0x00ff0000;
                        }
                    }
                }
            }
        }
    else {//自分が攻撃
        for(i=0;i>=2;i++){//全体
            if(i != clientID){
                if(gClients[i].ADsta == 1)
                    if(gClients[i].poi.x - gClients[clientID].poi.x <= 40 && gClients[clientID].poi.x - gClients[i].poi.x >= -40)
                        if(gClients[i].poi.y - gClients[clientID].poi.y <= 40 && gClients[clientID].poi.y - gClients[i].poi.y >= -40){
                            color = 0x00ff0000;
                        }
            }
        }
    }
}

*/

