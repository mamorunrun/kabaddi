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

#define PORT			(u_short)8888	/* ポート番号 */

#define MAX_CLIENTS		2				/* クライアント数の最大値 */
#define MAX_NAME_SIZE	10 				/* ユーザー名の最大値*/

#define MAX_DATA		200				/* 送受信するデータの最大値 */

#define END_COMMAND		'E'		  		/* プログラム終了コマンド */
#define ROCK_COMMAND	'R'				/* rockコマンド */
#define PAPER_COMMAND	'P'				/* paperコマンド */
#define SCISSORS_COMMAND	'S'				/* scissorsコマンド */
#define WIN_COMMAND 'W'    /*勝利コマンド*/
#define LOSE_COMMAND 'L'   /*敗北コマンド*/
#define DRAW_COMMAND 'D'   /*引き分けコマンド*/
#endif
