/*****************************************************************
�ե�����̾	: client_win.c
��ǽ		: ���饤����ȤΥ桼�������󥿡��ե���������
*****************************************************************/

#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_gfxPrimitives.h>
#include"common.h"
#include"client_func.h"

static SDL_Surface *gMainWindow;
static SDL_Rect gButtonRect[MAX_CLIENTS+2];

static int CheckButtonNO(int x,int y,int num);

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
	SDL_Surface *image;
	char rockButton[]="rock.png";
	char paperButton[]="paper.png";
	char scissorsButton[]="scissors.png";
	char endButton[]="END.jpg";
	char *s,title[10];

    /* �����������å� */
    assert(0<num && num<=MAX_CLIENTS);
	
	/* SDL�ν���� */
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("failed to initialize SDL.\n");
		return -1;
	}
	
	/* �ᥤ��Υ�����ɥ���������� */
	if((gMainWindow = SDL_SetVideoMode(400,240, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		return -1;
	}
	/* ������ɥ��Υ����ȥ�򥻥å� */
	sprintf(title,"Janken[%d]",clientID);
	SDL_WM_SetCaption(title,NULL);

	
	/* �طʤ���ˤ��� */
	SDL_FillRect(gMainWindow,NULL,0xffffff);

	/* �ܥ���κ��� */
	for(i=0;i<3;i++){
		gButtonRect[i].x = 25+125*i;
		gButtonRect[i].y=10;
		gButtonRect[i].w=100;
		gButtonRect[i].h=60;
      
		if(i==0){
			s=rockButton;
		}
		else if(i==1){
			s=paperButton;
		}
		else if(i==2){
			s=scissorsButton;
		}
		image = IMG_Load(s);
		SDL_BlitSurface(image,NULL,gMainWindow,&(gButtonRect[i]));
		SDL_FreeSurface(image);
	}
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

/*****************************************************************
�ؿ�̾	: WindowEvent
��ǽ	: �ᥤ�󥦥���ɥ����Ф��륤�٥�Ƚ�����Ԥ�
����	: int		num		: �����饤����ȿ�
����	: �ʤ�
*****************************************************************/
void WindowEvent(int num)
{
	SDL_Event event;
	SDL_MouseButtonEvent *mouse;
	int buttonNO;

    /* �����������å� */
    assert(0<num && num<=MAX_CLIENTS);

	if(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				SendEndCommand();
				break;
			case SDL_MOUSEBUTTONUP:
				mouse = (SDL_MouseButtonEvent*)&event;
				if(mouse->button == SDL_BUTTON_LEFT){
					buttonNO = CheckButtonNO(mouse->x,mouse->y,num);
#ifndef NDEBUG
					printf("#####\n");
					printf("WindowEvent()\n");
					printf("Button %d is pressed\n",buttonNO);
#endif
					if(buttonNO==0){
						SendRockCommand();
					}
					else if(buttonNO==1){
						SendPaperCommand();
					}
					else if(buttonNO==2){
						SendScissorsCommand();
					}
				}
				break;
		}
	}
}

/*****
static
*****/
/*****************************************************************
�ؿ�̾	: CheckButtonNO
��ǽ	: ����å����줿�ܥ�����ֹ���֤�
����	: int	   x		: �ޥ����β����줿 x ��ɸ
		  int	   y		: �ޥ����β����줿 y ��ɸ
		  char	   num		: �����饤����ȿ�
����	: �����줿�ܥ�����ֹ���֤�
		  �ܥ��󤬲�����Ƥ��ʤ�����-1���֤�
*****************************************************************/
static int CheckButtonNO(int x,int y,int num)
{
	int i;

 	for(i=0;i<3;i++){
		if(gButtonRect[i].x < x &&
			gButtonRect[i].y < y &&
      		gButtonRect[i].x + gButtonRect[i].w > x &&
			gButtonRect[i].y + gButtonRect[i].h > y){
			return i;
		}
	}
 	return -1;
}

void WinDisplay(void)
{
    /* �����ǡ����ؤΥݥ��� */
    SDL_Surface *Win_image;

    /* ���������� */
    SDL_Rect src_rect = { 0, 0, 300, 150 };

    /* Ž���դ����� */ 
    SDL_Rect dst_rect = { 50, 80};

    /* �������ɤ߹��� */
    Win_image = IMG_Load("win.png");

    SDL_BlitSurface(Win_image, &src_rect, gMainWindow, &dst_rect);

    SDL_Flip(gMainWindow);
    SDL_Delay(2000);

}

void LoseDisplay(void)
{
    /* �����ǡ����ؤΥݥ��� */
    SDL_Surface *Lose_image;

    /* ���������� */
    SDL_Rect src_rect = { 0, 0, 300, 150 };

    /* Ž���դ����� */ 
    SDL_Rect dst_rect = { 50, 80};

    /* �������ɤ߹��� */
    Lose_image = IMG_Load("lose.png");

    SDL_BlitSurface(Lose_image, &src_rect, gMainWindow, &dst_rect);

    SDL_Flip(gMainWindow);
    SDL_Delay(2000);

}

void DrawDisplay(void)
{
    /* �����ǡ����ؤΥݥ��� */
    SDL_Surface *Draw_image;

    /* ���������� */
    SDL_Rect src_rect = { 0, 0, 300, 150 };

    /* Ž���դ����� */ 
    SDL_Rect dst_rect = { 50, 80};

    /* �������ɤ߹��� */
    Draw_image = IMG_Load("draw.png");

    SDL_BlitSurface(Draw_image, &src_rect, gMainWindow, &dst_rect);
    
    SDL_Flip(gMainWindow);
}
