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

    /*送られてきたデータをコンマごとに区切る*/
    char *app_id = strtok(command, ",");   //データのID　kabaddi か endkabaddi
    int com = atoi(strtok(NULL, ","));     //コマンド
    int id=atoi(strtok(NULL, ","));        //クライアントID
    int x=atoi(strtok(NULL, ","));         //キャラのx座標
    int y=atoi(strtok(NULL, ","));         //キャラのy座標
    int t=atoi(strtok(NULL, ","));         //残り時間
    int rect_x=atoi(strtok(NULL, ","));    //画像を貼り付けるx座標
    int rect_y=atoi(strtok(NULL, ","));    //画像を貼り付けるy座標
    int rect_w=atoi(strtok(NULL, ","));    //画像を貼り付ける長さ
    int	endFlag = 1;              

    int i;
    int j;

    /*データの先頭が"endkabaddi"であればゲーム終了*/
    if(strcmp("endkabaddi",app_id)==0)
        return 0;
    /*データの先頭が"kabaddi"でないならそれは間違ったデータなのでリターンする*/
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
            UpdatePos(id,x,y,t,rect_x,rect_y,rect_w);
        break;
    case TIMES://ゲーム回数を全クライアントに送る
        gametimes=x;
        buttonflag=1;
        break;
    case BUMP://当たり判定
        if(gClients[x].color==0){
            gClients[id].Bflag++;
            gClients[x].Bflag++; 
            gClients[x].color=4;
            gClients[id].score = gClients[id].score + gClients[x].Bflag;
            if(chara_rect[x].w ==192)
                chara_rect[x].y += 288;
            else if(chara_rect[x].x <=480)
                chara_rect[x].x+=576;
        }
        break;
    case RESTART:
        j = 0;
        if(game.flag == x)
            gClients[id].restart=1;//相手のrestartを1にする
        else
            break;
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
            case 3: {//各ピリオド終了からゲームへ
                if(loop % (cnum*gametimes) != 0){
                    game.flag = 1;
                    loop++; 
                }
                else {
                    game.flag = 4;
                    game.restTime = 1;
                }
            }
                break;
            case 4: game.flag = 0;//エンドからメイン画面へ
                break;
            }
        }
        break;
    case TACKLE://タックルに成功した守備から送られる
        for(i=0;i<cnum;i++){
            if(gClients[i].ADsta == 1){//攻撃の人間に
                gClients[i].tackle++;
                printf("receive tackle command\ngClients[%d].tackle=%d\n\n\n",i,gClients[i].tackle);
            }
        }
        break;
    case SCORE:
        gClients[id].score=x;
        game.flag=3;
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
   
    sprintf(data,"endkabaddi,a,a,a,a,a,a,a,a\0");
    /* データの送信 */
    SendData(data);
}


