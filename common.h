/*****************************************************************
ファイル名	: common.h
機能		: サーバーとクライアントで使用する定数の宣言を行う
*****************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<assert.h>
#include<math.h>

#include <SDL/SDL.h>	// SDLを用いるために必要なヘッダファイル
#include <SDL/SDL_gfxPrimitives.h>	// 描画関係のヘッダファイル
#include <libcwiimote/wiimote.h>	// Wiiリモコンを用いるために必要なヘッダファイル
#include <libcwiimote/wiimote_api.h>	// Wiiリモコンを用いるために必要なヘッダファイル

#define PORT			(u_short)8888	/* ポート番号 */

#define MAX_CLIENTS		3				/* クライアント数の最大値 */
#define MAX_NAME_SIZE	10 				/* ユーザー名の最大値*/

#define MAX_DATA		200				/* 送受信するデータの最大値 */

#define END_COMMAND		'E'		  		/* プログラム終了コマンド */
#define POS_COMMAND	'P'				/* posコマンド */
#define CDRAW           'C'                             /*charadrawコマンド*/

/* クライアントを表す構造体 */
typedef struct{
	int		fd;
	char	name[MAX_NAME_SIZE];
        SDL_Rect poi;
}CLIENT;

extern CLIENT	gClients[MAX_CLIENTS];


#endif
