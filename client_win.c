/*****************************************************************
ファイル名	: client_win.c
機能		: クライアントのユーザーインターフェース処理
*****************************************************************/

#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_gfxPrimitives.h>
#include"common.h"
#include"client_func.h"

static SDL_Surface *gMainWindow;
static SDL_Rect gButtonRect[MAX_CLIENTS+2];

static int CheckButtonNO(int x,int y,int num);

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
	SDL_Surface *image;
	char rockButton[]="rock.png";
	char paperButton[]="paper.png";
	char scissorsButton[]="scissors.png";
	char endButton[]="END.jpg";
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
	/* ウインドウのタイトルをセット */
	sprintf(title,"Janken[%d]",clientID);
	SDL_WM_SetCaption(title,NULL);

	
	/* 背景を白にする */
	SDL_FillRect(gMainWindow,NULL,0xffffff);

	/* ボタンの作成 */
	for(i=0;i<3;i++){
		gButtonRect[i].x = 25+125*i;
		gButtonRect[i].y=10;
		gButtonRect[i].w=100;
		gButtonRect[i].h=60;
      
		if(i==0){
			s=rockButton;
		}
		else if(i==1){
			s=paperButton;
		}
		else if(i==2){
			s=scissorsButton;
		}
		image = IMG_Load(s);
		SDL_BlitSurface(image,NULL,gMainWindow,&(gButtonRect[i]));
		SDL_FreeSurface(image);
	}
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

/*****************************************************************
関数名	: WindowEvent
機能	: メインウインドウに対するイベント処理を行う
引数	: int		num		: 全クライアント数
出力	: なし
*****************************************************************/
void WindowEvent(int num)
{
	SDL_Event event;
	SDL_MouseButtonEvent *mouse;
	int buttonNO;

    /* 引き数チェック */
    assert(0<num && num<=MAX_CLIENTS);

	if(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				SendEndCommand();
				break;
			case SDL_MOUSEBUTTONUP:
				mouse = (SDL_MouseButtonEvent*)&event;
				if(mouse->button == SDL_BUTTON_LEFT){
					buttonNO = CheckButtonNO(mouse->x,mouse->y,num);
#ifndef NDEBUG
					printf("#####\n");
					printf("WindowEvent()\n");
					printf("Button %d is pressed\n",buttonNO);
#endif
					if(buttonNO==0){
						SendRockCommand();
					}
					else if(buttonNO==1){
						SendPaperCommand();
					}
					else if(buttonNO==2){
						SendScissorsCommand();
					}
				}
				break;
		}
	}
}

/*****
static
*****/
/*****************************************************************
関数名	: CheckButtonNO
機能	: クリックされたボタンの番号を返す
引数	: int	   x		: マウスの押された x 座標
		  int	   y		: マウスの押された y 座標
		  char	   num		: 全クライアント数
出力	: 押されたボタンの番号を返す
		  ボタンが押されていない時は-1を返す
*****************************************************************/
static int CheckButtonNO(int x,int y,int num)
{
	int i;

 	for(i=0;i<3;i++){
		if(gButtonRect[i].x < x &&
			gButtonRect[i].y < y &&
      		gButtonRect[i].x + gButtonRect[i].w > x &&
			gButtonRect[i].y + gButtonRect[i].h > y){
			return i;
		}
	}
 	return -1;
}

void WinDisplay(void)
{
    /* 画像データへのポインタ */
    SDL_Surface *Win_image;

    /* 画像サイズ */
    SDL_Rect src_rect = { 0, 0, 300, 150 };

    /* 貼り付け位置 */ 
    SDL_Rect dst_rect = { 50, 80};

    /* 画像の読み込み */
    Win_image = IMG_Load("win.png");

    SDL_BlitSurface(Win_image, &src_rect, gMainWindow, &dst_rect);

    SDL_Flip(gMainWindow);
    SDL_Delay(2000);

}

void LoseDisplay(void)
{
    /* 画像データへのポインタ */
    SDL_Surface *Lose_image;

    /* 画像サイズ */
    SDL_Rect src_rect = { 0, 0, 300, 150 };

    /* 貼り付け位置 */ 
    SDL_Rect dst_rect = { 50, 80};

    /* 画像の読み込み */
    Lose_image = IMG_Load("lose.png");

    SDL_BlitSurface(Lose_image, &src_rect, gMainWindow, &dst_rect);

    SDL_Flip(gMainWindow);
    SDL_Delay(2000);

}

void DrawDisplay(void)
{
    /* 画像データへのポインタ */
    SDL_Surface *Draw_image;

    /* 画像サイズ */
    SDL_Rect src_rect = { 0, 0, 300, 150 };

    /* 貼り付け位置 */ 
    SDL_Rect dst_rect = { 50, 80};

    /* 画像の読み込み */
    Draw_image = IMG_Load("draw.png");

    SDL_BlitSurface(Draw_image, &src_rect, gMainWindow, &dst_rect);
    
    SDL_Flip(gMainWindow);
}
