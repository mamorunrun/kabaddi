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

// タイマー関数でスタミナを管理
Uint32 callbackfunc(Uint32 interval, void *param){
    game.restTime = game.restTime - 100;
    return interval;
    }

typedef struct//フレームレート用の変数
{
Uint32 now,//現在時間
       wit,//待ち時間
       lev;//経過時間
}timers;
extern timers timer;
extern int dflag;       //攻守反転フラグ
extern int gametimes;   //ゲームを繰り返す回数,スタート画面で設定
static int endFlag=1;   //メインループ終了条件
int ima;                //アニメーション用
int loop;               //現在のゲーム回数を保存
int clientID;           //自分のID
int cnum;               //全クライアント数

// Wiiリモコンを用いるための構造体を宣言（初期化）
wiimote_t wiimote = WIIMOTE_INIT;	// Wiiリモコンの状態格納用
wiimote_report_t report = WIIMOTE_REPORT_INIT;	// レポートタイプ用

SDL_Thread *thr_net;//コマンド用マルチスレッド
Game game;
SDL_TimerID timer_id1;	//タイマ割り込みを行うためのタイマのID

/**********************************************
マルチスレッドでサーバーからのコマンド処理を行う
 **********************************************/
static int thread_net(void *data)
{
    while(endFlag){
	endFlag = SendRecvManager();
    }
    return 0;
}


int main(int argc,char *argv[])
{


    timers timer;
    int i;//for文でcnum分繰り返すため
    char	name[MAX_CLIENTS][MAX_NAME_SIZE];
    char	localHostName[]="localhost";
    char	*serverName;
    char	data[MAX_DATA];//サーバーとのコマンド用
    

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
    
    wiimote.mode.acc = 1;

    switch(clientID){
    case 0: wiimote.led.one   = 1; break;
    case 1: wiimote.led.two   = 1; break; 
    case 2 :wiimote.led.three = 1; break;
    case 3 :wiimote.led.four  = 1; break;
    default: break;
        }

    

    // SDL_GetTicks関数を用いる時間管理
    Uint32 next_frame=SDL_GetTicks();	// SDLライブラリの初期化からの経過ミリ秒数を取得
    game.flag = 0;//ゲームフラグの初期化
    thr_net=SDL_CreateThread(thread_net,NULL);
    timer_id1=SDL_AddTimer(100, callbackfunc, NULL);

/*********************************************************
メインイベントループ game.flag : 0スタート画面ループ
                                1ゲーム画面作成
                                2ゲーム画面ループ
                                3各ピリオド終了
                                4エンド画面
***********************************************************/ 
    while(endFlag){

        timer.wit=timer.now-timer.lev;//待ち時間を計算

        /********スタート画面ループ**************/
        if(game.flag == 0){
            loop=1;//loopフラグの初期化
            gametimes=3;//gametimesを初期化,その後Topwindow内で変更可
            TopWindow();//スタート画面
            while(game.flag == 0){
                WindowEvent(clientID,ima);
                //全クライアントがAを押すとclient_comand内でgame.flagが1になる
                if(endFlag == 0)
                    break;
            }
        }
        else if(game.flag == 1){//ゲーム画面作成
            
            if(GameWindows(clientID,name,loop)==-1){
                fprintf(stderr,"setup failed : GameWindows\n");
		return -1;}
        }
        /*********ゲーム画面ループ************/
        else if(game.restTime > 0 && game.flag == 2){
            WindowEvent(clientID,ima);  
            ima=SDL_GetTicks();//現在時間を取得
            DrawChara(clientID,cnum);
        }
        
        else if(game.flag == 3 || game.restTime <= 0)
        {
            game.flag = 3;
            if(gClients[clientID].ADsta==1 && game.restTime<=0){//スタミナ切れでゲームが終了した時の得点の調整
                gClients[clientID].score -= gClients[clientID].Bflag;
                sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d,%d\0",SCORE,clientID,gClients[clientID].score,0,0,0,0,0);
                SendData(data);
                }
            WinDisplay(clientID);//得点の表示
            while(game.flag == 3){
                WindowEvent(clientID,ima);
                //全クライアントがAを押すとclient_comand内でgame.flagが変わる
                if(endFlag == 0)
                    break;
            }
        }
        else if(game.flag == 4){
            EndWindow();
            //エンド画面の表示
            
            /*for(i=0;i<cnum;i++){
                gClients[i].score=0;
                }*/
            while(game.flag == 4){
                WindowEvent(clientID,ima);
                //全クライアントが2を押すとgame.flagが0になり,スタート画面ループに戻る
                if(endFlag == 0)
                    break;
            }    
            
        }
        
        timer.lev=SDL_GetTicks();//経過時間を更新
    }
    
    /* 終了処理 */
    SDL_RemoveTimer(timer_id1);
    DestroyWindow();
    CloseSoc();
    
    return 0;
}
