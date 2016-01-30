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
#define MAX_NAME_SIZE	100 				/* �桼����̾�κ�����*/

#define MAX_DATA		200				/* ����������ǡ����κ����� */

#define END_COMMAND		'E'		  		/* �ץ���ཪλ���ޥ�� */
#define POS_COMMAND	'P'				/* pos���ޥ�� */
#define CDRAW           'C'                             /*charadraw���ޥ��*/
#define BUMP            'B'                             /*������Ƚ��Υ��ޥ��*/
#define WIN             'W'                             /*�������ޥ��*/
#define LOSE            'L'                             /*�餱���ޥ��   ���롩*/
#define TACKLE          'T'                             /*���å��륳�ޥ��*/
#define RESTART         'R'                             /*�ꥹ�����ȥ��ޥ��*/

extern int clientID; 
typedef struct {
    int restTime;               /* �Ĥ���� */
    int flag;                   /* ������ξ��� */
    }Game;

extern Game game;

/* ���饤����Ȥ�ɽ����¤�� */
typedef struct{
	int		fd;
	char	name[MAX_NAME_SIZE];
        SDL_Rect poi;
        int anime;  /*���˥᡼�����ե饰*/
        int anipatnum; /*���˥᡼�����ѥ�����*/
        int             ADsta;//��������Υե饰��ATK1 DEF0
        int             Bflag;//������Ƚ��Υե饰
        int             score;//��������¸
        int             color;
        int             restart;
}CLIENT;

extern CLIENT	gClients[MAX_CLIENTS];

#endif
