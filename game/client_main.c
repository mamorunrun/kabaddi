/*****************************************************************
ファイル名	: client_main.c
機能		: クライアントのメインルーチン
*****************************************************************/

#include"common.h"
#include"client_func.h"

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <libcwiimote/wiimote.h>
#include <libcwiimote/wiimote_api.h>

// タイマーで呼び出されるコールバック関数
Uint32 callbackfunc(Uint32 interval, void *param){
    //   game.restTime--;
    DisplayStatus();
    return interval;
}

typedef struct//フレームレート用の変数
{
Uint32 now,//現在時間
       wit,//待ち時間
       lev;//経過時間
}timers;
extern timers timer;
extern int dflag;//移動したことの検知
static int endFlag=1;

int clientID;
int cnum;

// Wiiリモコンを用いるための構造体を宣言（初期化）
wiimote_t wiimote = WIIMOTE_INIT;	// Wiiリモコンの状態格納用
wiimote_report_t report = WIIMOTE_REPORT_INIT;	// レポートタイプ用

SDL_Thread *thr_net;
SDL_Thread *thr_time;
Game game;
SDL_TimerID timer_id1;	//　タイマ割り込みを行うためのタイマのID

static int thread_net(void *data)
{
    while(endFlag){
	endFlag = SendRecvManager();
    }
    return 0;
}

static int thread_time(void *data){
    timer_id1=SDL_AddTimer(100, callbackfunc, NULL);
   
    return 0;
}

int main(int argc,char *argv[])
{


    timers timer;

    int		num;
    int loop = 0;
    char	name[MAX_CLIENTS][MAX_NAME_SIZE];
    char	localHostName[]="localhost";
    char	*serverName;
    //int		clientID;

    //  SDL_TimerID timer_id1;	//　タイマ割り込みを行うためのタイマのID

    // SDL初期化
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("failed to initialize SDL.\n");
        exit(-1);
    }

    /* 引き数チェック */
    if(argc == 2){
    	serverName = localHostName;
        if (wiimote_connect(&wiimote, argv[1]) < 0) {	// コマンド引数に指定したWiiリモコン識別情報を渡して接続
            printf("unable to open wiimote: %s\n", wiimote_get_error());
            exit(1);
    }
    }
    else if(argc == 3){
    	serverName = argv[1];
        if (wiimote_connect(&wiimote, argv[2]) < 0) {	// コマンド引数に指定したWiiリモコン識別情報を渡して接続
            printf("unable to open wiimote: %s\n", wiimote_get_error());
            exit(1);
    }
    }
    else{
		fprintf(stderr, "Usage: %s, Cannot find a Server Name.\n", argv[0]);
		return -1;
    }

    /* サーバーとの接続 */
    if(SetUpClient(serverName,&clientID,name)==-1){
		fprintf(stderr,"setup failed : SetUpClient\n");
		return -1;
	}



    printf("clientnum=%d\n",cnum);

    /* スタート画面の初期化 */
    /*if(InitWindows()==-1){
		fprintf(stderr,"setup failed : InitWindows\n");
		return -1;
                }*/
    
   /*ゲームウィンドウの初期化*/
    if(GameWindows(clientID,name,loop)==-1){
	fprintf(stderr,"setup failed : GameWindows\n");
		return -1;

    }

        printf("clientnum=%d\n",cnum);

        wiimote.mode.acc = 1;

        switch(clientID){
        case 0: wiimote.led.one =   1; break;
        case 1: wiimote.led.two   = 1; break; 
        case 2 :wiimote.led.three = 1; break;

        default: break;
        }



    /*wiiリモコンの入力受付開始*/

    // SDL_GetTicks関数を用いる時間管理
	Uint32 next_frame=SDL_GetTicks();	// SDLライブラリの初期化からの経過ミリ秒数を取得
        
        dflag = 0;
        game.flag = 0;
        thr_net=SDL_CreateThread(thread_net,NULL);
        thr_time=SDL_CreateThread(thread_time,NULL);
    /* メインイベントループ */
    while(endFlag){
/*
        timer.now=SDL_GetTicks();//現在時間を取得
        timer.wit=timer.now-timer.lev;//待ち時間を計算

        if(dflag == 0)
        {
            WindowEvent(clientID);
            //timer.lev=SDL_GetTicks();//経過時間を更新
        }
        else if(timer.wit > 16){
            WindowEvent(clientID);
            dflag = 0;
            timer.lev=SDL_GetTicks();//経過時間を更新
        }
*/
        timer.now=SDL_GetTicks();//現在時間を取得
        timer.wit=timer.now-timer.lev;//待ち時間を計算

        /*  if(timer.wit > 1000){
            //        game.restTime--;
            timer.lev=SDL_GetTicks();//経過時間を更新
            }*/

        
        
        if(game.restTime > 0 && game.flag == 0){
            WindowEvent(clientID);  
            printf("game.restTime:%d\n",game.restTime);
            DrawChara(clientID,cnum);
            
            
        }
        
        else{
            game.flag = 1;
            loop++;
            DrawChara(clientID,cnum);
            while(game.flag == 1){
                WindowEvent(clientID);
                
            }
            
            if(GameWindows(clientID,name,loop)==-1){
                fprintf(stderr,"setup failed : GameWindows\n");
		return -1;
                
            }
            timer.lev=SDL_GetTicks();//経過時間を更新
        }
       

        // timer.now=SDL_GetTicks();//現在時間を取得
        //timer.wit=timer.now-timer.lev;//待ち時間を計算
        
        // if(timer.wit<16)
            //    SDL_Delay(16-timer.wit);//16以下ならCPUを休ませる
        
        //timer.lev=SDL_GetTicks();//経過時間を更新

    }

    /* 終了処理 */
    SDL_RemoveTimer(timer_id1);
	DestroyWindow();
	CloseSoc();

    return 0;
}
