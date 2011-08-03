#ifndef _SCORES_H
#define _SCORES_H


#include "basescene.h"
#include "IwResManager.h"
#include "IwTexture.h"


class CScores: public CBaseScene
{
private:
	//Scoreloop variables
	SC_ScoreController_h m_scoreController;
	SC_ScoresController_h m_scoresController;
	SC_RankingController_h m_rankingController;
	SC_UserController_h m_userController;
	SC_ScoreList_h m_scores;
	CGUIListView *m_listview;
	int m_curuserrank;
	int m_curuserscore;
	int m_curuserminorresult;

	void FillLocalScoresTable();


	static void ScoresControllerCallback(void *userData, SC_Error_t completionStatus);
	void OnRequestSuccess();
	void OnRequestFailed(SC_Error_t error);

	static void ScoreControllerCallbackSubmit(void *userData, SC_Error_t completionStatus);
	void OnRequestSuccessSubmit();
	void OnRequestFailedSubmit(SC_Error_t error);
	void DoSubmitScore();

	static void RankingControllerCallback(void *userData, SC_Error_t completionStatus);
	void OnRequestSuccessRanking();
	void OnRequestFailedRanking(SC_Error_t error);

	void OnRequestSuccessUser();
	void OnRequestFailedUser(SC_Error_t error);
	static void UserControllerCallback(void *userData, SC_Error_t completionStatus);

protected:
	void ControlPress(int index);
public:
	CScores();
	~CScores();
	void Render();
	void Update(int deltaTimeMs);
	bool Load();
	void UnLoad();
	void Show();
};


#endif //_SCORES_H