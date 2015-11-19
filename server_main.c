/*****************************************************************
ファイル名	: server_main.c
機能		: サーバーのメインルーチン
*****************************************************************/

#include<SDL/SDL.h>
#include"server_common.h"

int main(int argc,char *argv[])
{
	int	num=2;
	int	endFlag = 1;

        SDL_TimerID timer_id;


		
	/* SDLの初期化 */
	if(SDL_Init(SDL_INIT_TIMER) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	/* クライアントとの接続 */
	if(SetUpServer(num) == -1){
		fprintf(stderr,"Cannot setup server\n");
		exit(-1);
	}

	timer_id=SDL_AddTimer(100, callbackfunc, &animation);
	/* メインイベントループ */
	while(endFlag){
		endFlag = SendRecvManager();
	};

	/* 終了処理 */
	Ending();

	return 0;
}
