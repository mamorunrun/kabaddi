/*****************************************************************
�ե�����̾	: client_func.h
��ǽ		: ���饤����Ȥγ����ؿ������
*****************************************************************/

#ifndef _CLIENT_FUNC_H_
#define _CLIENT_FUNC_H_

#include"common.h"

//wii��⥳��
#include <libcwiimote/wiimote.h>// Wii��⥳����Ѥ��뤿���ɬ�פʥإå��ե�����
#include <libcwiimote/wiimote_api.h>// Wii��⥳����Ѥ��뤿���ɬ�פʥإå��ե�����

/* client_net.c */
extern int SetUpClient(char* hostName,int *clientID,int *num,char clientName[][MAX_NAME_SIZE]);
extern void CloseSoc(void);
extern int RecvIntData(int *intData);
extern void SendData(void *data,int dataSize);
extern int SendRecvManager(void);

/* client_win.c */
extern int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE]);
extern void DestroyWindow(void);
extern void DrawChara(int n, int x, int y);

/* client_command.c */
extern int ExecuteCommand(char command);


/*�岼�����Υ��ޥ�ɤ�����ؿ�*/
extern void SendUpCommand(void);
extern void SendDownCommand(void);
extern void SendRightCommand(void);
extern void SendLeftCommand(void);

extern void SendEndCommand(void);

#endif
