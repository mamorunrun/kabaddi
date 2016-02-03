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
Uint32 callbackfunc(Uint32 interval, void *param){
    game.restTime = game.restTime - 100;
    //DisplayStatus();
    return interval;
    }

typedef struct//�ե졼��졼���Ѥ��ѿ�
{
Uint32 now,//���߻���
       wit,//�Ԥ�����
       lev;//�в����
}timers;
extern timers timer;
extern int dflag;//��ư�������Ȥθ���
extern int gametimes;//������򷫤��֤����
static int endFlag=1;
int ima;
int loop;
int clientID;
int cnum;

// Wii��⥳����Ѥ��뤿��ι�¤�Τ�����ʽ������
wiimote_t wiimote = WIIMOTE_INIT;	// Wii��⥳��ξ��ֳ�Ǽ��
wiimote_report_t report = WIIMOTE_REPORT_INIT;	// ��ݡ��ȥ�������

SDL_Thread *thr_net;
//SDL_Thread *thr_time;
Game game;
SDL_TimerID timer_id1;	//�������޳����ߤ�Ԥ�����Υ����ޤ�ID

static int thread_net(void *data)
{
    while(endFlag){
	endFlag = SendRecvManager();
    }
    return 0;
}

/*static int thread_time(void *data){
    timer_id1=SDL_AddTimer(1000, callbackfunc, NULL);
   
    return 0;
    }*/

int main(int argc,char *argv[])
{


    timers timer;
    int lflag=0;
    int		num;
    int i;//cnum�����֤�����
    char	name[MAX_CLIENTS][MAX_NAME_SIZE];
    char	localHostName[]="localhost";
    char	*serverName;
    char	data[MAX_DATA];
    //int		clientID;

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
    if(SetUpClient(serverName,&clientID,name)==-1){
		fprintf(stderr,"setup failed : SetUpClient\n");
		return -1;
	}

    printf("clientnum=%d\n",cnum);

    /* �������Ȳ��̤ν���� */
    /*if(InitWindows()==-1){
		fprintf(stderr,"setup failed : InitWindows\n");
		return -1;
                }*/
    
   /*�����०����ɥ��ν����*/
    /*  if(GameWindows(clientID,name,loop)==-1){
	fprintf(stderr,"setup failed : GameWindows\n");
		return -1;

                }*/

        printf("clientnum=%d\n",cnum);

        wiimote.mode.acc = 1;

        switch(clientID){
        case 0: wiimote.led.one =   1; break;
        case 1: wiimote.led.two   = 1; break; 
        case 2 :wiimote.led.three = 1; break;

        default: break;
        }

    /*wii��⥳������ϼ��ճ���*/

    // SDL_GetTicks�ؿ����Ѥ�����ִ���
	Uint32 next_frame=SDL_GetTicks();	// SDL�饤�֥��ν��������ηв�ߥ��ÿ������
        
        //dflag = 0;
        game.flag = 0;
        thr_net=SDL_CreateThread(thread_net,NULL);
        //thr_time=SDL_CreateThread(thread_time,NULL);
        timer_id1=SDL_AddTimer(100, callbackfunc, NULL);
    /* �ᥤ�󥤥٥�ȥ롼�� */
    while(endFlag){
/*
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
*/
        timer.wit=timer.now-timer.lev;//�Ԥ����֤�׻�

        /*if(timer.wit > 1000){
            game.restTime--;
            timer.lev=SDL_GetTicks();//�в���֤򹹿�
            }*/


        /********�ᥤ����̥롼��**************/
        if(game.flag == 0){
            loop=1;
            gametimes=3;
            TopWindow();
            printf("now==%d\n\n",game.flag);
            while(game.flag == 0){
                WindowEvent(clientID,ima);
                if(endFlag == 0)
                    break;
            }
        }
        else if(game.flag == 1){//��������̺���
            
            if(GameWindows(clientID,name,loop)==-1){
                fprintf(stderr,"setup failed : GameWindows\n");
		return -1;}
        }
        /*********��������̥롼��************/
        else if(game.restTime > 0 && game.flag == 2){
            WindowEvent(clientID,ima);  
            ima=SDL_GetTicks();//���߻��֤����
            //printf("game.restTime:%d\n",game.restTime);
//>>>>>>> color
            DrawChara(clientID,cnum);
        }
        
        else if(game.flag == 3 || game.restTime <= 0)
        {
            game.flag = 3;
            if(gClients[clientID].ADsta==1 && game.restTime<=0){
                gClients[clientID].score -= gClients[clientID].Bflag;
                sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d,%d\0",SCORE,clientID,gClients[clientID].score,0,0,0,0,0);
                SendData(data);
                }
            WinDisplay(clientID);
            
            printf("now==%dloop==%dcnum==%dgametimes=%d\n\n",game.flag,loop,cnum,gametimes);
            while(game.flag == 3){
                WindowEvent(clientID,ima);
                if(endFlag == 0)
                    break;
            }
        }
        else if(game.flag == 4){
            //loop=0;
            for(i=0;i<cnum;i++){
                gClients[i].score=0;
                gClients[i].restart=0;
                // gametimes=3;
            }
            EndWindow();
            while(game.flag == 4){
                WindowEvent(clientID,ima);
                if(endFlag == 0)
                    break;
            }    
            
        }
        
        timer.lev=SDL_GetTicks();//�в���֤򹹿�
        
        
        
        
        // timer.now=SDL_GetTicks();//���߻��֤����
        //timer.wit=timer.now-timer.lev;//�Ԥ����֤�׻�
        
        // if(timer.wit<16)
            //    SDL_Delay(16-timer.wit);//16�ʲ��ʤ�CPU��٤ޤ���
        
        //timer.lev=SDL_GetTicks();//�в���֤򹹿�
        
    }
    
    /* ��λ���� */
    SDL_RemoveTimer(timer_id1);
    DestroyWindow();
    CloseSoc();
    
    return 0;
}
