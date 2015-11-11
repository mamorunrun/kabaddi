/*****************************************************************
�ե�����̾	: server_command.c
��ǽ		: �����С��Υ��ޥ�ɽ���
*****************************************************************/

#include"server_common.h"
#include"server_func.h"

static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);


static void Judge(int i,int pos);
static void SendJudge(int pos);

/*****************************************************************
�ؿ�̾	: ExecuteCommand
��ǽ	: ���饤����Ȥ��������Ƥ������ޥ�ɤ򸵤ˡ�
		  ����������������¹Ԥ���
����	: char	command		: ���ޥ��
		  int	pos			: ���ޥ�ɤ����ä����饤������ֹ�
����	: �ץ���ཪλ���ޥ�ɤ������Ƥ������ˤ�0���֤���
		  ����ʳ���1���֤�
*****************************************************************/
int ExecuteCommand(char command,int pos)
{
    unsigned char	data[MAX_DATA];
    int			dataSize,intData;
    int			endFlag = 1;

    /* �����������å� */
    assert(0<=pos && pos<MAX_CLIENTS);

#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("Get command %c\n",command);
#endif
    switch(command){
    case END_COMMAND:
        dataSize = 0;
        /* ���ޥ�ɤΥ��å� */
        SetCharData2DataBlock(data,command,&dataSize);
        
        /* ���桼���������� */
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
        /* ̤�ΤΥ��ޥ�ɤ������Ƥ��� */
        fprintf(stderr,"0x%02x is not command!\n",command);
    }
    return endFlag;
}

/*****
static
*****/
/*****************************************************************
�ؿ�̾	: SetIntData2DataBlock
��ǽ	: int ���Υǡ����������ѥǡ����κǸ�˥��åȤ���
����	: void		*data		: �����ѥǡ���
		  int		intData		: ���åȤ���ǡ���
		  int		*dataSize	: �����ѥǡ����θ��ߤΥ�����
����	: �ʤ�
*****************************************************************/
static void SetIntData2DataBlock(void *data,int intData,int *dataSize)
{
    int tmp;

    /* �����������å� */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    tmp = htonl(intData);

    /* int ���Υǡ����������ѥǡ����κǸ�˥��ԡ����� */
    memcpy(data + (*dataSize),&tmp,sizeof(int));
    /* �ǡ��������������䤹 */
    (*dataSize) += sizeof(int);
}

/*****************************************************************
�ؿ�̾	: SetCharData2DataBlock
��ǽ	: char ���Υǡ����������ѥǡ����κǸ�˥��åȤ���
����	: void		*data		: �����ѥǡ���
		  int		intData		: ���åȤ���ǡ���
		  int		*dataSize	: �����ѥǡ����θ��ߤΥ�����
����	: �ʤ�
*****************************************************************/
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* �����������å� */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    /* int ���Υǡ����������ѥǡ����κǸ�˥��ԡ����� */
    *(char *)(data + (*dataSize)) = charData;
    /* �ǡ��������������䤹 */
    (*dataSize) += sizeof(char);
}

/*****************************************************************
�ؿ�̾	: Judge
��ǽ	: �ɤä��Υ��饤����Ȥ����ä��Τ���Ƚ��
����	: int hand :�Ф����꤬hand0=Rock,1=Paper,2=Scissors
          int pos  :���ޥ�ɤ����ä����饤������ֹ�
����	: �ʤ�
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
            SendJudge(ALL_CLIENTS);//����ʬ��
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
�ؿ�̾	: SendJudge
��ǽ	: ���ԤΥǡ����򥯥饤����Ȥ�����
����	: int	pos	���Ԥ�(3�ξ��ϰ���ʬ��)
����	: �ʤ�
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
        /* ���ޥ�ɤΥ��å� */
        SetCharData2DataBlock(data2,LOSE_COMMAND,&dataSize);
        /* �ǡ��������� */
        SendData(0,data2,dataSize);

        dataSize = 0;
        /* ���ޥ�ɤΥ��å� */
        SetCharData2DataBlock(data1,WIN_COMMAND,&dataSize);
        /* �ǡ��������� */
        SendData(1,data1,dataSize);
        
    }
    else if(pos == 0)
    {
        dataSize = 0;
        /* ���ޥ�ɤΥ��å� */
        SetCharData2DataBlock(data2,LOSE_COMMAND,&dataSize);
        /* �ǡ��������� */
        SendData(1,data2,dataSize);

        dataSize = 0;
        /* ���ޥ�ɤΥ��å� */
        SetCharData2DataBlock(data1,WIN_COMMAND,&dataSize);
        /* �ǡ��������� */
        SendData(0,data1,dataSize);

    }
    else if(pos == ALL_CLIENTS)
    {
        dataSize = 0;
        /* ���ޥ�ɤΥ��å� */
        SetCharData2DataBlock(data3,DRAW_COMMAND,&dataSize);
        /* �ǡ��������� */
        SendData(ALL_CLIENTS,data3,dataSize);
    }
}
