/*****************************************************************
ファイル名	: client_func.h
機能		: クライアントの外部関数の定義
*****************************************************************/

#ifndef _CLIENT_FUNC_H_
#define _CLIENT_FUNC_H_

#include"common.h"

//wiiリモコン
#include <libcwiimote/wiimote.h>// Wiiリモコンを用いるために必要なヘッダファイル
#include <libcwiimote/wiimote_api.h>// Wiiリモコンを用いるために必要なヘッダファイル
extern wiimote_t wiimote;
extern wiimote_report_t report;
extern int dflag;//移動したことの検知main,winで使用

extern int dirflag; //client_win.cキャラの方向
extern int cnum;  /*クライアントの数*/

enum{
    up_dir          = 0, 
    up_right_dir    = 1, 
    right_dir       = 2,
    right_down_dir  = 3,
    down_dir        = 4,
    down_left_dir   = 5,
    left_dir        = 6,
    left_up_dir     = 7
};

/* client_net.c */
extern int SetUpClient(char* hostName,int *clientID,char clientName[][MAX_NAME_SIZE]);
extern void CloseSoc(void);
extern void SendData(char *data);
extern int SendRecvManager(void);

/* client_win.c */
extern int  InitWindows(void);
extern int  GameWindows(int clientID,char name[][MAX_NAME_SIZE], int loop);
/*自分のID,全員の名前,何回目のループか*/
extern void DestroyWindow(void);
extern void DrawChara(int n,int cnum);

extern void WindowEvent(int clientID);
extern void WinDisplay(int clientID);

/*client_system.c*/
extern void UpdatePos(int n,int x,int y,int t);
extern void Move(int clientID,int befx,int befy);
//当たり判定
extern int Collision(int clientID,int befx,int befy);

/* client_command.c */
extern int ExecuteCommand(char *command);


/*上下左右のコマンドを送る関数*/

extern void SendEndCommand(void);

#endif
