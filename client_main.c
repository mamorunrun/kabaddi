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

// �����ޡ��ؿ��ǥ����ߥʤ����
Uint32 callbackfunc(Uint32 interval, void *param){
    game.restTime = game.restTime - 100;
    return interval;
    }

typedef struct//�ե졼��졼���Ѥ��ѿ�
{
Uint32 now,//���߻���
       wit,//�Ԥ�����
       lev;//�в����
}timers;
extern timers timer;
extern int dflag;       //����ȿž�ե饰
extern int gametimes;   //������򷫤��֤����,�������Ȳ��̤�����
static int endFlag=1;   //�ᥤ��롼�׽�λ���
int ima;                //���˥᡼�������
int loop;               //���ߤΥ�����������¸
int clientID;           //��ʬ��ID
int cnum;               //�����饤����ȿ�

// Wii��⥳����Ѥ��뤿��ι�¤�Τ�����ʽ������
wiimote_t wiimote = WIIMOTE_INIT;	// Wii��⥳��ξ��ֳ�Ǽ��
wiimote_report_t report = WIIMOTE_REPORT_INIT;	// ��ݡ��ȥ�������

SDL_Thread *thr_net;//���ޥ���ѥޥ������å�
Game game;
SDL_TimerID timer_id1;	//�����޳����ߤ�Ԥ�����Υ����ޤ�ID

/**********************************************
�ޥ������åɤǥ����С�����Υ��ޥ�ɽ�����Ԥ�
 **********************************************/
static int thread_net(void *data)
{
    while(endFlag){
	endFlag = SendRecvManager();
    }
    return 0;
}


int main(int argc,char *argv[])
{


    timers timer;
    int i;//forʸ��cnumʬ�����֤�����
    char	name[MAX_CLIENTS][MAX_NAME_SIZE];
    char	localHostName[]="localhost";
    char	*serverName;
    char	data[MAX_DATA];//�����С��ȤΥ��ޥ����
    

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
    
    wiimote.mode.acc = 1;

    switch(clientID){
    case 0: wiimote.led.one   = 1; break;
    case 1: wiimote.led.two   = 1; break; 
    case 2 :wiimote.led.three = 1; break;
    case 3 :wiimote.led.four  = 1; break;
    default: break;
        }

    

    // SDL_GetTicks�ؿ����Ѥ�����ִ���
    Uint32 next_frame=SDL_GetTicks();	// SDL�饤�֥��ν��������ηв�ߥ��ÿ������
    game.flag = 0;//������ե饰�ν����
    thr_net=SDL_CreateThread(thread_net,NULL);
    timer_id1=SDL_AddTimer(100, callbackfunc, NULL);

/*********************************************************
�ᥤ�󥤥٥�ȥ롼�� game.flag : 0�������Ȳ��̥롼��
                                1��������̺���
                                2��������̥롼��
                                3�ƥԥꥪ�ɽ�λ
                                4����ɲ���
***********************************************************/ 
    while(endFlag){

        timer.wit=timer.now-timer.lev;//�Ԥ����֤�׻�

        /********�������Ȳ��̥롼��**************/
        if(game.flag == 0){
            loop=1;//loop�ե饰�ν����
            gametimes=3;//gametimes������,���θ�Topwindow����ѹ���
            TopWindow();//�������Ȳ���
            while(game.flag == 0){
                WindowEvent(clientID,ima);
                //�����饤����Ȥ�A�򲡤���client_comand���game.flag��1�ˤʤ�
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
            DrawChara(clientID,cnum);
        }
        
        else if(game.flag == 3 || game.restTime <= 0)
        {
            game.flag = 3;
            if(gClients[clientID].ADsta==1 && game.restTime<=0){//�����ߥ��ڤ�ǥ����ब��λ��������������Ĵ��
                gClients[clientID].score -= gClients[clientID].Bflag;
                sprintf(data,"kabaddi,%d,%d,%d,%d,%d,%d,%d,%d\0",SCORE,clientID,gClients[clientID].score,0,0,0,0,0);
                SendData(data);
                }
            WinDisplay(clientID);//������ɽ��
            while(game.flag == 3){
                WindowEvent(clientID,ima);
                //�����饤����Ȥ�A�򲡤���client_comand���game.flag���Ѥ��
                if(endFlag == 0)
                    break;
            }
        }
        else if(game.flag == 4){
            EndWindow();
            //����ɲ��̤�ɽ��
            
            /*for(i=0;i<cnum;i++){
                gClients[i].score=0;
                }*/
            while(game.flag == 4){
                WindowEvent(clientID,ima);
                //�����饤����Ȥ�2�򲡤���game.flag��0�ˤʤ�,�������Ȳ��̥롼�פ����
                if(endFlag == 0)
                    break;
            }    
            
        }
        
        timer.lev=SDL_GetTicks();//�в���֤򹹿�
    }
    
    /* ��λ���� */
    SDL_RemoveTimer(timer_id1);
    DestroyWindow();
    CloseSoc();
    
    return 0;
}
