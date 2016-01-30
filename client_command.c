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
/*����������*/
    case CDRAW:
        printf("pos");
        if(id != clientID)
            UpdatePos(id,x,y,t,rect_x,rect_y);
        break;
    case TIMES://���������������饤����Ȥ�����
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
        if(gClients[x].color==2)//�����ο���2�ʤ�
        {
            gClients[id].score = gClients[id].score + gClients[x].Bflag;
            gClients[x].color=4;//2��Ʊ������WIN���ޥ�ɤ�롼�פ���Τ��ɤ�����
        }
        break;
    case RESTART:
        j = 0;
        gClients[id].restart=1;//����restart��1�ˤ���
        for(i=0;i<cnum;i++){
            if(gClients[i].restart == 1)
                j++;
        }
        if(j == cnum){
            for(i=0;i<cnum;i++){
                gClients[i].restart=0;
            }
            
            switch(game.flag){
            case 0: game.flag = 1;//�ᥤ�󤫤饲�����
                break;
            case 1: game.flag = 0;
                break;
            case 2: game.flag = 0;
                break;
            case 3: game.flag = 1;//�ƥԥꥪ�ɽ�λ���饲�����
                break;
                
            case 4: game.flag = 0;//����ɤ���ᥤ����̤�
                break;
            }
        }
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
   
    sprintf(data,"endkabaddi,a,a,a,a,a,a,a\0");
    /* �ǡ��������� */
    SendData(data);
}


