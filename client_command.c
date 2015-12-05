/*****************************************************************
�ե�����̾	: client_command.c
��ǽ		: ���饤����ȤΥ��ޥ�ɽ���
*****************************************************************/

#include"common.h"
#include"client_func.h"

static void RecCharaData(void);
/*****************************************************************
�ؿ�̾	: ExecuteCommand
��ǽ	: �����С����������Ƥ������ޥ�ɤ򸵤ˡ�
		  ����������������¹Ԥ���
����	: char	command		: ���ޥ��
����	: �ץ���ཪλ���ޥ�ɤ��������Ƥ������ˤ�0���֤���
		  ����ʳ���1���֤�
*****************************************************************/
int ExecuteCommand(char *command)
{
    char *app_id = strtok(command, ",");
    int com = atoi(strtok(NULL, ","));

    int id=atoi(strtok(NULL, ","));
    int x=atoi(strtok(NULL, ","));
    int y=atoi(strtok(NULL, ","));
    int	endFlag = 1;


    printf("%s\n",app_id);
    printf("app_id = %s\n",app_id);
    printf("id=%d\n",id)
    printf("x=%d\n",x);
    printf("y=%d\n",host);

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
        break;
/*����������*/
    case CDRAW:
        printf("pos");
        UpdatePos(id,x,y);
        break;
    }
    return endFlag;
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
    char	data[MAX_DATA];
    int			dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendEndCommand()\n");
#endif
   
    sprintf(data,"endkabaddi,a,a,a,a\0");
    /* �ǡ��������� */
    SendData(data);
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
void SetIntData2DataBlock(void *data,int intData,int *dataSize)
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
void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* �����������å� */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    /* char ���Υǡ����������ѥǡ����κǸ�˥��ԡ����� */
    *(char *)(data + (*dataSize)) = charData;
    /* �ǡ��������������䤹 */
    (*dataSize) += sizeof(char);
}


/*****************************************************************
����������������������������������������������������������������������
��������������������chara�Υǡ�����������
******************************************************************/

static void RecCharaData(void)
{
    int	n,x,y;

    /* �ߥ��ޥ�ɤ��Ф����������������� */
    RecvIntData(&n);
    RecvIntData(&x);
    RecvIntData(&y);

    printf("n=%d, x= %d, y=%d\n",n,x,y);

    /* �����򹹿����� */
    UpdatePos(n,x,y);
}
