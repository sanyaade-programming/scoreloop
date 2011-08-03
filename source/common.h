#ifndef _COMMON_H
#define _COMMON_H

#include "sc_client.h"
#include "gui.h"

//Scoreloop properties
#define SC_TEST_GAME_ID         "cc03ae79-534c-4253-b1d9-346749dad0d6"
#define SC_TEST_GAME_SECRET     "vLHQH7VZvP9zl6QTlCkDQG9WvUqX7cOu5EPw5mJZHHp2DN+/MwwWhA=="
#define SC_PAGE_SCORES_COUNT    100
#define LOCAL_SCORES_COUNT    10

//Scoreloop Client variable
extern SC_Client_h g_client;

enum SceneType { stNone, stMainMenu, stGame, stProfile, stScores};

struct CScoreLine
{
	int position;
	char name[128];
	int score;
};


extern SceneType g_curSceneType;
extern bool g_waiting;
extern char g_waitcaption[128];
extern CGUIDialog *g_errorDialog;

//extern int32 g_debugval;

extern bool g_localhighscoresubmited;
extern int g_localhighscore;
extern char g_localname[128];
extern CScoreLine g_localscores[LOCAL_SCORES_COUNT];


//Common functions
extern bool Save(); //Saving local scores
extern bool Load(); //Loading local scores
extern void FillDefaultLocalScores();
extern void SubmitLocalScore(int value);
extern int GetLocalScorePosition(int score);
extern void StartWait(const char *caption);
extern void FinishWait();

extern void ShowErrorDlg(const char *text);
extern void CloseErrorDlg();

#endif //_COMMON_H