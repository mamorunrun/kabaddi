/*****************************************************************
�ե�����̾	: client_command.c
��ǽ		: ���饤����ȤΥ��ޥ�ɽ���
*****************************************************************/

#include"common.h"
#include"client_func.h"


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
/*����������*/
    case CDRAW:
        printf("pos");
        if(id != clientID)
            UpdatePos(id,x,y);
        break;
    case BUMP:
        if(id != clientID){
            gClients[id].color=3;
            gClients[x].Bflag++;
            gClients[x].color=2;
        }
        break;
    case WIN:
        //WinDisplay();
        game.flag=1;
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


