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
extern wiimote_t wiimote;
extern wiimote_report_t report;
extern int dflag;//��ư�������Ȥθ���main,win�ǻ���

extern int dirflag; //client_win.c����������
extern int cnum;  /*���饤����Ȥο�*/

enum{
    up_dir    = 0, 
    right_dir = 1,
    down_dir  = 2,
    left_dir  = 3
};

/* client_net.c */
extern int SetUpClient(char* hostName,int *clientID,char clientName[][MAX_NAME_SIZE]);
extern void CloseSoc(void);
extern void SendData(char *data);
extern int SendRecvManager(void);

/* client_win.c */
extern int InitWindows(int clientID,char name[][MAX_NAME_SIZE]);
extern void DestroyWindow(void);
extern void DrawChara(int n,int cnum);
extern void UpdatePos(int n,int x,int y);
extern void WindowEvent(int clientID);
extern void Move(int clientID);
extern void Judge(int clientID,int cnum);//������Ƚ��


/* client_command.c */
extern int ExecuteCommand(char *command);


/*�岼�����Υ��ޥ�ɤ�����ؿ�*/

extern void SendEndCommand(void);

#endif
