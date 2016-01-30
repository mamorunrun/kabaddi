/*****************************************************************
ファイル名	: client_command.c
機能		: クライアントのコマンド処理
*****************************************************************/

#include"common.h"
#include"client_func.h"


/*****************************************************************
関数名	: ExecuteCommand
機能	: サーバーから送られてきたコマンドを元に，
		  引き数を受信し，実行する
引数	: char	command		: コマンド
出力	: プログラム終了コマンドがおくられてきた時には0を返す．
		  それ以外は1を返す
*****************************************************************/
int ExecuteCommand(char *command)
{
    char *app_id = strtok(command, ",");
    int com = atoi(strtok(NULL, ","));

    int id=atoi(strtok(NULL, ","));
    int x=atoi(strtok(NULL, ","));
    int y=atoi(strtok(NULL, ","));
    int t=atoi(strtok(NULL, ","));
    int rect_x=atoi(strtok(NULL, ","));
    int rect_y=atoi(strtok(NULL, ","));
    int	endFlag = 1;

    int i;
    int j;

    /*printf("%s\n",app_id);
    printf("app_id = %s\n",app_id);
    printf("id=%d\n",id);
    printf("x=%d\n",x);
    printf("y=%d\n",y);*/

    if(strcmp("endkabaddi",app_id)==0)
        return 0;
    else if(strcmp("kabaddi",app_id)!=0)
        return endFlag;

#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("command = %c\n",command);
#endif
    switch(com){
    case END_COMMAND:
        endFlag = 0;
        game.flag=0;
        break;
/*キャラを描画*/
    case CDRAW:
        printf("pos");
        if(id != clientID)
            UpdatePos(id,x,y,t,rect_x,rect_y);
        break;
    case TIMES://ゲーム回数を全クライアントに送る
        gametimes=x;
        buttonflag=1;
    case BUMP:
            if(gClients[x].color==0){
                gClients[id].Bflag++;
                gClients[x].Bflag++;
                gClients[id].color=3;
                gClients[x].color=2;
            }
            break;
    case WIN:
        if(gClients[x].color==2)//守備の色が2なら
        {
            gClients[id].score = gClients[id].score + gClients[x].Bflag;
            gClients[x].color=4;//2と同じ色でWINコマンドをループするのを防ぐため
        }
        break;
    case RESTART:
        j = 0;
        gClients[id].restart=1;//相手のrestartを1にする
        for(i=0;i<cnum;i++){
            if(gClients[i].restart == 1)
                j++;
        }
        if(j == cnum){
            for(i=0;i<cnum;i++){
                gClients[i].restart=0;
            }
            
            switch(game.flag){
            case 0: game.flag = 1;//メインからゲームへ
                break;
            case 1: game.flag = 0;
                break;
            case 2: game.flag = 0;
                break;
            case 3: game.flag = 1;//各ピリオド終了からゲームへ
                break;
                
            case 4: game.flag = 0;//エンドからメイン画面へ
                break;
            }
        }
        break;
    }
    return endFlag;
}


/*****************************************************************
関数名	: SendEndCommand
機能	: プログラムの終了を知らせるために，
		  サーバーにデータを送る
引数	: なし
出力	: なし
*****************************************************************/
void SendEndCommand(void)
{
    char	data[MAX_DATA];
    int			dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendEndCommand()\n");
#endif
   
    sprintf(data,"endkabaddi,a,a,a,a,a,a,a\0");
    /* データの送信 */
    SendData(data);
}


