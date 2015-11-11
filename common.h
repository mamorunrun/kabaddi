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

#define PORT			(u_short)8888	/* �ݡ����ֹ� */

#define MAX_CLIENTS		2				/* ���饤����ȿ��κ����� */
#define MAX_NAME_SIZE	10 				/* �桼����̾�κ�����*/

#define MAX_DATA		200				/* ����������ǡ����κ����� */

#define END_COMMAND		'E'		  		/* �ץ���ཪλ���ޥ�� */
#define ROCK_COMMAND	'R'				/* rock���ޥ�� */
#define PAPER_COMMAND	'P'				/* paper���ޥ�� */
#define SCISSORS_COMMAND	'S'				/* scissors���ޥ�� */
#define WIN_COMMAND 'W'    /*�������ޥ��*/
#define LOSE_COMMAND 'L'   /*���̥��ޥ��*/
#define DRAW_COMMAND 'D'   /*����ʬ�����ޥ��*/
#endif
