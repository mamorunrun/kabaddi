/*****************************************************************
�ե�����̾	: server_main.c
��ǽ		: �����С��Υᥤ��롼����
*****************************************************************/

#include<SDL/SDL.h>
#include"server_common.h"

aaaaa

int main(int argc,char *argv[])
{
	int	num=2;
	int	endFlag = 1;

		
	/* SDL�ν���� */
	if(SDL_Init(SDL_INIT_TIMER) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	/* ���饤����ȤȤ���³ */
	if(SetUpServer(num) == -1){
		fprintf(stderr,"Cannot setup server\n");
		exit(-1);
	}
	
	/* �ᥤ�󥤥٥�ȥ롼�� */
	while(endFlag){
		endFlag = SendRecvManager();
	};

	/* ��λ���� */
	Ending();

	return 0;
}
