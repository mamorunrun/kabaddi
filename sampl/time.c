#include <stdio.h>
#include <SDL/SDL.h>       // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL/SDL_image.h>       // SDL_imageを用いるために必要なヘッダファイルをインクルード
int callbackfunc1(int interval, void *param){
    printf("Callback Function 1 -- 0.5sec passed\n");
    return interval;
}

int callbackfunc2(int interval, void *param){
    int *times = (int*)param;
//受け取った引数（パラメータ）をキャスト演算子でintのポインタ型に変換し、変数timesに格納
    printf("Callback Function 2 -- 2sec Passed (%d times)\n", *times);
    return interval;
}


int main(int argc, char* argv[]){
    SDL_TimerID timer_id1, timer_id2;	//　タイマ割り込みを行うためのタイマのID
        
    	// SDL初期化
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("failed to initialize SDL.\n");
        exit(-1);
    }
    int times = 1;
    
    timer_id1 = SDL_AddTimer(500, callbackfunc1, NULL);
 
//0.5秒ごとにコールバック関数を呼び出す（引数なし）

    timer_id2 =SDL_AddTimer (2000, callbackfunc2, &times);
    
// 2秒ごとにコールバック関数を呼び出す（int型変数のアドレスを引数として渡す
    while(1){
        int n;
        scanf("%d",&n);
        if(n == 0) break;
        
        
    }
    SDL_RemoveTimer(timer_id1);
    SDL_RemoveTimer(timer_id2);

return 0;
}
