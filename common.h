/*****************************************************************
�ե�����̾	: common.h
��ǽ		: �����С��ȥ��饤����Ȥǻ��Ѥ�������������Ԥ�
*****************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<assert.h>
#include<math.h>

#include <SDL/SDL.h>	// SDL���Ѥ��뤿���ɬ�פʥإå��ե�����
#include <SDL/SDL_gfxPrimitives.h>	// ����ط��Υإå��ե�����
#include <libcwiimote/wiimote.h>	// Wii��⥳����Ѥ��뤿���ɬ�פʥإå��ե�����
#include <libcwiimote/wiimote_api.h>	// Wii��⥳����Ѥ��뤿���ɬ�פʥإå��ե�����

#define PORT			(u_short)8888	/* �ݡ����ֹ� */

#define MAX_CLIENTS		3				/* ���饤����ȿ��κ����� */
#define MAX_NAME_SIZE	10 				/* �桼����̾�κ�����*/

#define MAX_DATA		200				/* ����������ǡ����κ����� */

#define END_COMMAND		'E'		  		/* �ץ���ཪλ���ޥ�� */
#define POS_COMMAND	'P'				/* pos���ޥ�� */
#define CDRAW           'C'                             /*charadraw���ޥ��*/

/* ���饤����Ȥ�ɽ����¤�� */
typedef struct{
	int		fd;
	char	name[MAX_NAME_SIZE];
        SDL_Rect poi;
}CLIENT;

extern CLIENT	gClients[MAX_CLIENTS];


#endif
