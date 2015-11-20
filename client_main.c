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
//Uint32 callbackfunc(Uint32 interval, void *param){
//	return interval;
//}

typedef struct//フレームレート用の変数
{
Uint32 now,//現在時間
       wit,//待ち時間
       lev;//経過時間
}timers;
extern timers timer;

// Wiiリモコンを用いるための構造体を宣言（初期化）
wiimote_t wiimote = WIIMOTE_INIT;	// Wiiリモコンの状態格納用
wiimote_report_t report = WIIMOTE_REPORT_INIT;	// レポートタイプ用

int main(int argc,char *argv[])
{
    timers timer;

    int		num;
    char	name[MAX_CLIENTS][MAX_NAME_SIZE];
    int		endFlag=1;
    char	localHostName[]="localhost";
    char	*serverName;
    int		clientID;

//    SDL_TimerID timer_id1;	//　タイマ割り込みを行うためのタイマのID

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
    if(SetUpClient(serverName,&clientID,&num,name)==-1){
		fprintf(stderr,"setup failed : SetUpClient\n");
		return -1;
	}

    /* ウインドウの初期化 */
	if(InitWindows(clientID,num,name)==-1){
		fprintf(stderr,"setup failed : InitWindows\n");
		return -1;
	}

        printf("%d\n",clientID);

        wiimote.mode.acc = 1;
    /*wiiリモコンの入力受付開始*/

    // タイマの作成・設定（タイマIDの格納）
//        timer_id1=SDL_AddTimer(10, callbackfunc, NULL);	// 1秒ごとにコールバック関数を呼び出す（引数なし）
    // SDL_GetTicks関数を用いる時間管理
	Uint32 next_frame=SDL_GetTicks();	// SDLライブラリの初期化からの経過ミリ秒数を取得

    /* メインイベントループ */
    while(endFlag){
        WindowEvent(clientID);
        endFlag = SendRecvManager();

        timer.now=SDL_GetTicks();//現在時間を取得
        timer.wit=timer.now-timer.lev;//待ち時間を計算
        
        if(timer.wit<16)
            SDL_Delay(16-timer.wit);//16以下ならCPUを休ませる
        
        timer.lev=SDL_GetTicks();//経過時間を更新
        
        // if (SDL_GetTicks() >= next_frame) {	// 現在の経過時間が前より0.05秒以上経過していたら
        //    next_frame += 50;	// 現在の経過時間に50ms（0.05秒）をプラスして格納
            // endFlag = SendRecvManager();
        //   }
    };

    /* 終了処理 */
	DestroyWindow();
//	SDL_RemoveTimer(timer_id1);
        SDL_Quit();
	CloseSoc();

    return 0;
}
