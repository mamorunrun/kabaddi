/*****************************************************************
ファイル名	: server_command.c
機能		: サーバーのコマンド処理
*****************************************************************/

#include"server_common.h"
#include"server_func.h"

static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);


static void Judge(int i,int pos);
static void SendJudge(int pos);

/*****************************************************************
関数名	: ExecuteCommand
機能	: クライアントから送られてきたコマンドを元に，
		  引き数を受信し，実行する
引数	: char	command		: コマンド
		  int	pos			: コマンドを送ったクライアント番号
出力	: プログラム終了コマンドが送られてきた時には0を返す．
		  それ以外は1を返す
*****************************************************************/
int ExecuteCommand(char command,int pos)
{
    unsigned char	data[MAX_DATA];
    int			dataSize,intData;
    int			endFlag = 1;

    /* 引き数チェック */
    assert(0<=pos && pos<MAX_CLIENTS);

#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("Get command %c\n",command);
#endif
    switch(command){
    case END_COMMAND:
        dataSize = 0;
        /* コマンドのセット */
        SetCharData2DataBlock(data,command,&dataSize);
        
        /* 全ユーザーに送る */
        SendData(ALL_CLIENTS,data,dataSize);

        endFlag = 0;
        break;
    case ROCK_COMMAND:
        Judge(0,pos);
        break;
    case PAPER_COMMAND:
        Judge(1,pos);
	break;
    case SCISSORS_COMMAND:
        Judge(2,pos);
	break;
    default:
        /* 未知のコマンドが送られてきた */
        fprintf(stderr,"0x%02x is not command!\n",command);
    }
    return endFlag;
}

/*****
static
*****/
/*****************************************************************
関数名	: SetIntData2DataBlock
機能	: int 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
static void SetIntData2DataBlock(void *data,int intData,int *dataSize)
{
    int tmp;

    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    tmp = htonl(intData);

    /* int 型のデータを送信用データの最後にコピーする */
    memcpy(data + (*dataSize),&tmp,sizeof(int));
    /* データサイズを増やす */
    (*dataSize) += sizeof(int);
}

/*****************************************************************
関数名	: SetCharData2DataBlock
機能	: char 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    /* int 型のデータを送信用データの最後にコピーする */
    *(char *)(data + (*dataSize)) = charData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(char);
}

/*****************************************************************
関数名	: Judge
機能	: どっちのクライアントが勝ったのかを判断
引数	: int hand :出した手がhand0=Rock,1=Paper,2=Scissors
          int pos  :コマンドを送ったクライアント番号
出力	: なし
*****************************************************************/
static void Judge(int hand,int pos)//hand0=Rock,1=Paper,2=Scissors
{
    static int hand0 = -1,hand1 = -1;

    if(pos==0)
    {
    hand0 = hand;
    }
    else if(pos==1)
    {
    hand1 = hand;
    }

    if( hand0 != -1 && hand1 != -1){
        if(hand0 == hand1){
            SendJudge(ALL_CLIENTS);//引き分け
        }
        else if(hand0 == 0 && hand1 == 1){//pos=1 win
            SendJudge(1);
        }
        else if(hand0 == 1 && hand1 == 2){//pos=1 win
            SendJudge(1);
        }
        else if(hand0 == 2 && hand1 == 0){//pos=1 win
            SendJudge(1);
        }
        else if(hand0 == 1 && hand1 == 0){//pos=0 win
            SendJudge(0);
        }
        else if(hand0 == 2 && hand1 == 1){//pos=0 win
            SendJudge(0);
        }
        else if(hand0 == 0 && hand1 == 2){//pos=0 win
            SendJudge(0);
        }
    hand0 = -1;
    hand1 = -1;
    }
}

/*****************************************************************
関数名	: SendJudge
機能	: 勝者のデータをクライアントに送る
引数	: int	pos	勝者の(3の場合は引き分け)
出力	: なし
*****************************************************************/
void SendJudge(int pos)
{
    unsigned char	data1[MAX_DATA];
    unsigned char	data2[MAX_DATA];
    unsigned char	data3[MAX_DATA];
    int			dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendJudgeCommand()\n");
#endif

    if(pos == 1)
    {
        dataSize = 0;
        /* コマンドのセット */
        SetCharData2DataBlock(data2,LOSE_COMMAND,&dataSize);
        /* データの送信 */
        SendData(0,data2,dataSize);

        dataSize = 0;
        /* コマンドのセット */
        SetCharData2DataBlock(data1,WIN_COMMAND,&dataSize);
        /* データの送信 */
        SendData(1,data1,dataSize);
        
    }
    else if(pos == 0)
    {
        dataSize = 0;
        /* コマンドのセット */
        SetCharData2DataBlock(data2,LOSE_COMMAND,&dataSize);
        /* データの送信 */
        SendData(1,data2,dataSize);

        dataSize = 0;
        /* コマンドのセット */
        SetCharData2DataBlock(data1,WIN_COMMAND,&dataSize);
        /* データの送信 */
        SendData(0,data1,dataSize);

    }
    else if(pos == ALL_CLIENTS)
    {
        dataSize = 0;
        /* コマンドのセット */
        SetCharData2DataBlock(data3,DRAW_COMMAND,&dataSize);
        /* データの送信 */
        SendData(ALL_CLIENTS,data3,dataSize);
    }
}
