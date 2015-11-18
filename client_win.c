/*****************************************************************
�ե�����̾	: client_win.c
��ǽ		: ���饤����ȤΥ桼�������󥿡��ե���������
*****************************************************************/

#include<SDL/SDL.h>
#include<SDL/SDL_gfxPrimitives.h>
#include"common.h"
#include"client_func.h"

static SDL_Surface *gMainWindow;
static SDL_Surface *buffer;
CLIENT gClients[MAX_CLIENTS];

/*****************************************************************
�ؿ�̾	: InitWindows
��ǽ	: �ᥤ�󥦥���ɥ���ɽ���������Ԥ�
����	: int	clientID		: ���饤������ֹ�
		  int	num				: �����饤����ȿ�
����	: ���������Ǥ����Ȥ�0�����Ԥ����Ȥ�-1
*****************************************************************/
int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE])
{
	int i;
	char *s,title[10];

        /* �����������å� */
        assert(0<num && num<=MAX_CLIENTS);
	
	/* SDL�ν���� */
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("failed to initialize SDL.\n");
		return -1;
	}
	
	/* �ᥤ��Υ�����ɥ���������� */
	if((gMainWindow = SDL_SetVideoMode(1000,600, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		return -1;
	}
        if((buffer = SDL_CreateRGBSurface(SDL_SWSURFACE,1000,600,32,0,0,0,0))==NULL){
            printf("failed to initialize videomode.\n");
            exit(-1);
        }

	/* ������ɥ��Υ����ȥ�򥻥å� */
	sprintf(title,"Kabaddi[%d]",clientID);
	SDL_WM_SetCaption(title,NULL);

	
	/* �طʤ���ˤ��� */
	SDL_FillRect(buffer,NULL,0xffffffff);

        for(i=0;i<num;i++){
            gClients[i].poi.x=10*i+30;
            gClients[i].poi.y=10;
            gClients[i].poi.w=30;
            gClients[i].poi.h=30;

            SDL_FillRect(buffer,&gClients[i].poi,0x000000ff);
        }
        SDL_BlitSurface(buffer, NULL, gMainWindow, NULL);
	SDL_Flip(gMainWindow);
	
	return 0;
}

/*****************************************************************
�ؿ�̾	: DestroyWindow
��ǽ	: SDL��λ����
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
void DestroyWindow(void)
{
	SDL_Quit();
}

void WindowEvent(int clientID)
{
    unsigned char	data[MAX_DATA];
    int			dataSize = 0;
    int i;

    while (wiimote_is_open(&wiimote)){
        //memset(data,'NULL',MAX_DATA);
        if (wiimote_update(&wiimote) < 0) {
            wiimote_disconnect(&wiimote);
            break;
        }
        if (wiimote.keys.home) {
            printf("home\n");
            wiimote_speaker_free(&wiimote);
            wiimote_disconnect(&wiimote);
            SendEndCommand();
        }
        if(wiimote.keys.up){
            printf("up\n");
            gClients[clientID].poi.x--;
            SetCharData2DataBlock(data,POS_COMMAND,&dataSize);
            //SetIntData2DataBlock(data,clientID,&dataSize);
            printf("gClients[clientID].poi.x = %d\n",gClients[clientID].poi.x);
            SetIntData2DataBlock(data,gClients[clientID].poi.x,&dataSize);
            SetIntData2DataBlock(data,gClients[clientID].poi.y,&dataSize);
            //printf("%s,%d\n",data,data);
            SendData(data, dataSize);

        }
        else if (wiimote.keys.down){
            gClients[clientID].poi.x++;
            SetCharData2DataBlock(data,POS_COMMAND,&dataSize);
            //SetIntData2DataBlock(data,clientID,&dataSize);
            SetIntData2DataBlock(data,gClients[clientID].poi.x,&dataSize);
            SetIntData2DataBlock(data,gClients[clientID].poi.y,&dataSize);
            SendData(data, dataSize);
        }
        else if(wiimote.keys.left){
            gClients[clientID].poi.y++;
            SetCharData2DataBlock(data,POS_COMMAND,&dataSize);
            //SetIntData2DataBlock(data,clientID,&dataSize);
            SetIntData2DataBlock(data,gClients[clientID].poi.x,&dataSize);
            SetIntData2DataBlock(data,gClients[clientID].poi.y,&dataSize);
            SendData(data, dataSize);
        }
        else if(wiimote.keys.right){
            gClients[clientID].poi.y--;
            SetCharData2DataBlock(data,POS_COMMAND,&dataSize);
            // SetIntData2DataBlock(data,clientID,&dataSize);
            SetIntData2DataBlock(data,gClients[clientID].poi.x,&dataSize);
            SetIntData2DataBlock(data,gClients[clientID].poi.y,&dataSize);
            SendData(data, dataSize);
        }

        printf("%d %d %d\n",clientID,gClients[clientID].poi.x,gClients[clientID].poi.y);
        /*for(i=0;i<2;i++){
        printf("%d %d\n",gClients[i].poi.x,gClients[i].poi.y);
        }*/

        break;
       
    }
    
}

/*****
static
*****/

void DrawChara(int n, int x, int y)
{

    int i;
    int num=2;

    printf("%d %d %d\n",n,x,y);

    gClients[n].poi.x=x;
    gClients[n].poi.y=y;

    /* for(i=0;i<num;i++){
        printf("%d %d\n",gClients[i].poi.x,gClients[i].poi.y);
        }*/

    SDL_FillRect(buffer,NULL,0xffffffff);
    for(i=0;i<num;i++){
        SDL_FillRect(buffer,&gClients[i].poi,0x000000ff);
    }
    SDL_BlitSurface(buffer, NULL, gMainWindow, NULL);
    
    SDL_Flip(gMainWindow);
    
}

