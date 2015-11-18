/*****************************************************************
�ե�����̾	: client_main.c
��ǽ		: ���饤����ȤΥᥤ��롼����
*****************************************************************/

#include"common.h"
#include"client_func.h"
#include "system.h"

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <libcwiimote/wiimote.h>
#include <libcwiimote/wiimote_api.h>

/* �ѿ� */
int gWaitTime;

/* �ؿ� */
extern void TimerEvent( int time );

    // Wii��⥳����Ѥ��뤿��ι�¤�Τ�����ʽ������
wiimote_t wiimote = WIIMOTE_INIT;	// Wii��⥳��ξ��ֳ�Ǽ��
wiimote_report_t report = WIIMOTE_REPORT_INIT;	// ��ݡ��ȥ�������

int main(int argc,char *argv[])
{
    int		num;
    char	name[MAX_CLIENTS][MAX_NAME_SIZE];
    int		endFlag=1;
    char	localHostName[]="localhost";
    char	*serverName;
    int		clientID;

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

        printf("clientID = %d\n",clientID);
        printf("%client num = %d\n",num);

    /*wii��⥳������ϼ��ճ���*/
        wiimote.mode.acc = 1;

     /* �����ޡ� */
        Uint32 interval = SDL_GetTicks() + 100;

     /* ������������0.1�ä�1������� */
        int time = 0;

        Uint32 now;
    /* �ᥤ�󥤥٥�ȥ롼�� */
        while(endFlag){
    WindowEvent(clientID);
            /* �����ޡ����� */
            now = SDL_GetTicks();
            if( now >= interval ){
                TimerEvent( ++time );
                /* 0.05�ä��Ȥ˥����ޡ��������뤿�ᡤ0.05�ø������ */
                interval = now + 50
                    }
            
        };
        
        /* ��λ���� */
	DestroyWindow();
	CloseSoc();
        
        return 0;
}


/* �����ޡ�����
 *
 * ����
 *   time: ��������
 */
void TimerEvent( int time )
{
    endFlag = SendRecvManager();
   
}
