/*****************************************************************
�ե�����̾	: client_main.c
��ǽ		: ���饤����ȤΥᥤ��롼����
*****************************************************************/

#include"common.h"
#include"client_func.h"

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <libcwiimote/wiimote.h>
#include <libcwiimote/wiimote_api.h>

// �����ޡ��ǸƤӽФ���륳����Хå��ؿ�
//Uint32 callbackfunc(Uint32 interval, void *param){
//	return interval;
//}

typedef struct//�ե졼��졼���Ѥ��ѿ�
{
Uint32 now,//���߻���
       wit,//�Ԥ�����
       lev;//�в����
}timers;
extern timers timer;
extern int dflag;//��ư�������Ȥθ���

// Wii��⥳����Ѥ��뤿��ι�¤�Τ�����ʽ������
wiimote_t wiimote = WIIMOTE_INIT;	// Wii��⥳��ξ��ֳ�Ǽ��
wiimote_report_t report = WIIMOTE_REPORT_INIT;	// ��ݡ��ȥ�������

int main(int argc,char *argv[])
{
    timers timer;

    int		num;
    char	name[MAX_CLIENTS][MAX_NAME_SIZE];
    int		endFlag=1;
    char	localHostName[]="localhost";
    char	*serverName;
    int		clientID;

//    SDL_TimerID timer_id1;	//�������޳����ߤ�Ԥ�����Υ����ޤ�ID

    // SDL�����
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("failed to initialize SDL.\n");
        exit(-1);
    }

    /* �����������å� */
    if(argc == 2){
    	serverName = localHostName;
        if (wiimote_connect(&wiimote, argv[1]) < 0) {	// ���ޥ�ɰ����˻��ꤷ��Wii��⥳���̾�����Ϥ�����³
            printf("unable to open wiimote: %s\n", wiimote_get_error());
            exit(1);
    }
    }
    else if(argc == 3){
    	serverName = argv[1];
        if (wiimote_connect(&wiimote, argv[2]) < 0) {	// ���ޥ�ɰ����˻��ꤷ��Wii��⥳���̾�����Ϥ�����³
            printf("unable to open wiimote: %s\n", wiimote_get_error());
            exit(1);
    }
    }
    else{
		fprintf(stderr, "Usage: %s, Cannot find a Server Name.\n", argv[0]);
		return -1;
    }

    /* �����С��Ȥ���³ */
    if(SetUpClient(serverName,&clientID,&num,name)==-1){
		fprintf(stderr,"setup failed : SetUpClient\n");
		return -1;
	}

    /* ������ɥ��ν���� */
	if(InitWindows(clientID,num,name)==-1){
		fprintf(stderr,"setup failed : InitWindows\n");
		return -1;
	}

        printf("%d\n",clientID);

        wiimote.mode.acc = 1;
    /*wii��⥳������ϼ��ճ���*/

    // SDL_GetTicks�ؿ����Ѥ�����ִ���
	Uint32 next_frame=SDL_GetTicks();	// SDL�饤�֥��ν��������ηв�ߥ��ÿ������
        
        dflag = 0;
    /* �ᥤ�󥤥٥�ȥ롼�� */
    while(endFlag){

        endFlag = SendRecvManager();
        timer.now=SDL_GetTicks();//���߻��֤����
        timer.wit=timer.now-timer.lev;//�Ԥ����֤�׻�

        if(dflag == 0)
        {
            WindowEvent(clientID);
            //timer.lev=SDL_GetTicks();//�в���֤򹹿�
        }
        else if(timer.wit > 16){
            WindowEvent(clientID);
            dflag = 0;
            timer.lev=SDL_GetTicks();//�в���֤򹹿�
        }

       
        // timer.now=SDL_GetTicks();//���߻��֤����
        //timer.wit=timer.now-timer.lev;//�Ԥ����֤�׻�
        
        // if(timer.wit<16)
            //    SDL_Delay(16-timer.wit);//16�ʲ��ʤ�CPU��٤ޤ���
        
        //timer.lev=SDL_GetTicks();//�в���֤򹹿�

    };

    /* ��λ���� */
	DestroyWindow();
        SDL_Quit();
	CloseSoc();

    return 0;
}
