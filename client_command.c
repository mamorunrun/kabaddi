/*****************************************************************
�ե�����̾	: client_command.c
��ǽ		: ���饤����ȤΥ��ޥ�ɽ���
*****************************************************************/

#include"common.h"
#include"client_func.h"

static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);

/*****************************************************************
�ؿ�̾	: ExecuteCommand
��ǽ	: �����С����������Ƥ������ޥ�ɤ򸵤ˡ�
		  ����������������¹Ԥ���
����	: char	command		: ���ޥ��
����	: �ץ���ཪλ���ޥ�ɤ��������Ƥ������ˤ�0���֤���
		  ����ʳ���1���֤�
*****************************************************************/
int ExecuteCommand(char command)
{
    int	endFlag = 1;
#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("command = %c\n",command);
#endif
    switch(command){
    case END_COMMAND:
        endFlag = 0;
        break;
    case WIN_COMMAND:
        WinDisplay();
        endFlag = 0;
        break;
    case LOSE_COMMAND:
        LoseDisplay();
        endFlag = 0;
        break;
    case DRAW_COMMAND:
        DrawDisplay();
        break;
    }
    return endFlag;
}

/****************************************************************
�������������������岼�����Υǡ���������ؿ����ѹ�
����������������������������������������������������������������������
 ****************************************************************/


/*****************************************************************
�ؿ�̾	: SendUpCommand
��ǽ	: ��Υǡ����򥵡��С��˥ǡ���������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
void SendUpCommand(void)
{   
    unsigned char	data[MAX_DATA];
    int			dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendUpCommand()\n");
#endif
    dataSize = 0;
    /* ���ޥ�ɤΥ��å� */
    SetCharData2DataBlock(data,UP_COMMAND,&dataSize);

    /* �ǡ��������� */
    SendData(data,dataSize);
}

/*****************************************************************
�ؿ�̾	: SendDownCommand
��ǽ	: ���Υǡ����򥵡��С��˥ǡ���������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
void SendDownCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendDownCommand()\n");
#endif

    dataSize = 0;
    /* ���ޥ�ɤΥ��å� */
    SetCharData2DataBlock(data,DOWN_COMMAND,&dataSize);

    /* �ǡ��������� */
    SendData(data,dataSize);
}

/*****************************************************************
�ؿ�̾	: SendRightCommand
��ǽ	: ���Υǡ����򥵡��С��˥ǡ���������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
void SendRightCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendRightCommand()\n");
#endif

    dataSize = 0;
    /* ���ޥ�ɤΥ��å� */
    SetCharData2DataBlock(data,RIGHT_COMMAND,&dataSize);

    /* �ǡ��������� */
    SendData(data,dataSize);
}

/*****************************************************************
�ؿ�̾	: SendRightCommand
��ǽ	: ���Υǡ����򥵡��С��˥ǡ���������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
void SendLeftCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendLeftCommand()\n");
#endif

    dataSize = 0;
    /* ���ޥ�ɤΥ��å� */
    SetCharData2DataBlock(data,LEFT_COMMAND,&dataSize);

    /* �ǡ��������� */
    SendData(data,dataSize);
}

/*****************************************************************
�ؿ�̾	: SendEndCommand
��ǽ	: �ץ����ν�λ���Τ餻�뤿��ˡ�
		  �����С��˥ǡ���������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
void SendEndCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendEndCommand()\n");
#endif
    dataSize = 0;
    /* ���ޥ�ɤΥ��å� */
    SetCharData2DataBlock(data,END_COMMAND,&dataSize);

    /* �ǡ��������� */
    SendData(data,dataSize);
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

    /* char ���Υǡ����������ѥǡ����κǸ�˥��ԡ����� */
    *(char *)(data + (*dataSize)) = charData;
    /* �ǡ��������������䤹 */
    (*dataSize) += sizeof(char);
}
