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

enum{
    up_dir    = 0, 
    right_dir = 1,
    down_dir  = 2,
    left_dir  = 3
};

/* client_net.c */
extern int SetUpClient(char* hostName,int *clientID,int *num,char clientName[][MAX_NAME_SIZE]);
extern void CloseSoc(void);
extern int RecvIntData(int *intData);
extern void SendData(char *data);
extern int SendRecvManager(void);

/* client_win.c */
extern int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE]);
extern void DestroyWindow(void);
extern void DrawChara(int n);
extern void UpdatePos(int n,int x,int y);
extern void WindowEvent(int clientID);
extern int  Judge(int clientID);

/* client_command.c */
extern int ExecuteCommand(char *command);


/*上下左右のコマンドを送る関数*/

extern void SendEndCommand(void);

extern void SetIntData2DataBlock(void *data,int intData,int *dataSize);
extern void SetCharData2DataBlock(void *data,char charData,int *dataSize);

#endif
