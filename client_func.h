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
extern int gametimes;//������򷫤��֤����
extern int buttonflag;

extern int dirflag; //client_win.c����������
extern int cnum;  /*���饤����Ȥο�*/
extern SDL_Rect chara_rect[MAX_CLIENTS];
extern int tflag;//���å���Υե饰
extern int Ax,Ay,Af;//���⤬���ݥ���Ȱ�ư������
extern int loop;//loop�򥰥��Х��
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

enum{
    Atx=-15,
    Aty=42,
    Atw=96,
    Ath=58,
    Dfx=33,
    Dfy=57,
    Dfw=30,
    Dfh=30,
    Talx=0,
    Tarx=70,
    Talry=55,
    Talrw=122,
    Talrh=52,
    Taudx=24,
    Tauy=22,
    Tady=52,
    Taudw=53,
    Taudh=65
};

/* client_net.c */
extern int SetUpClient(char* hostName,int *clientID,char clientName[][MAX_NAME_SIZE]);
extern void CloseSoc(void);
extern void SendData(char *data);
extern int SendRecvManager(void);

/* client_win.c */
extern int  InitWindows(void);
extern int  GameWindows(int clientID,char name[][MAX_NAME_SIZE], int loop);
/*��ʬ��ID,������̾��,�����ܤΥ롼�פ�*/
extern void DestroyWindow(void);
extern void DrawChara(int n,int cnum);
extern int TopWindow(void);
extern int EndWindow(void);

extern void WindowEvent(int clientID,int now);
extern void WinDisplay(int clientID);

/*client_system.c*/
extern void UpdatePos(int n,int x,int y,int t,int rect_x,int rect_y,int rect_w);
extern void Move(int clientID,int befx,int befy,int now);
extern void Animation(int now);

//������Ƚ��
extern int Collision(int clientID,int befx,int befy);

/* client_command.c */
extern int ExecuteCommand(char *command);


/*�岼�����Υ��ޥ�ɤ�����ؿ�*/

extern void SendEndCommand(void);

#endif
