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


    // Wiiリモコンを用いるための構造体を宣言（初期化）
wiimote_t wiimote = WIIMOTE_INIT;	// Wiiリモコンの状態格納用
wiimote_report_t report = WIIMOTE_REPORT_INIT;	// レポートタイプ用

int main(int argc,char *argv[])
{
    int		num;
    char	name[MAX_CLIENTS][MAX_NAME_SIZE];
    int		endFlag=1;
    char	localHostName[]="localhost";
    char	*serverName;
    int		clientID;

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

    /* メインイベントループ */
    while(endFlag){
        WindowEvent(clientID);
        nanosleep(100);
        endFlag = SendRecvManager();
    };

    /* 終了処理 */
	DestroyWindow();
	CloseSoc();

    return 0;
}
