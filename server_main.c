/*****************************************************************
ファイル名	: server_main.c
機能		: サーバーのメインルーチン
*****************************************************************/

#include<SDL/SDL.h>
#include"server_common.h"

/*Uint32 callbackfunc(Uint32 interval,void *param){

    param=SendRecvManager();
    return interval;

    }*/



int main(int argc,char *argv[])
{
	int	num=2;
	int	endFlag = 1;

        int timer=0;


		
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

	/* メインイベントループ */
	while(endFlag){
            if(timer%10==0){
            endFlag=SendRecvManager();
            printf("a\n");
            }
            timer++;
            printf("%d\n",timer);
	};

	/* 終了処理 */
	Ending();

	return 0;
}
