#include "scores.h"
#include "common.h"
#include "render.h"

char g_scusername[64];



const char *g_OScaptions[] = {"Unknown", "Symbian OS", "Qualcomm BREW", "MS Windows", "MS Windows Mobile",
						"Linux", "SKT or KTF WIPI", "Nintendo DS", "ARM Semihosted environment", "Nucleus-based RTOS",
						"PRIVATE", "MS Windows CE", "Sharp device", "Apple OSX", "iOS",
						"UIQ OS", "PS3 OS", "XBOX 360 OS", "bada OS", "Android OS", "Palm webOS", "PSP OS", "Wii OS"};


CScores::CScores(): CBaseScene()
{
	m_listview = 0;

	// Creating Scoreloop Controllers
	SC_Error_t retCode = SC_Client_CreateScoresController(g_client, &m_scoresController,
														  ScoresControllerCallback, this);
	if (retCode != SC_OK) {
		m_scoresController = NULL;
		s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP, "Failed to create SC_ScoresController");
	}
	SC_Client_CreateScoreController(g_client, &m_scoreController,
											  ScoreControllerCallbackSubmit, this);
	if (retCode != SC_OK) {
		m_scoreController = NULL;
		s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP, "Failed to create SC_ScoreController");
	}

	SC_Client_CreateRankingController(g_client, &m_rankingController,
											  RankingControllerCallback, this);
	if (retCode != SC_OK) {
		m_rankingController = NULL;
		s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP, "Failed to create SC_RankingController");


	}

	SC_Client_CreateUserController(g_client, &m_userController,
											  UserControllerCallback, this);
	if (retCode != SC_OK) {
		m_userController = NULL;
		s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP, "Failed to create SC_UserController");


	}
}

CScores::~CScores()
{
	UnLoad();
	if (m_scoresController)
		SC_ScoresController_Release(m_scoresController);
	if (m_scoreController)
		SC_ScoreController_Release(m_scoreController);
	if (m_rankingController)
		SC_RankingController_Release(m_rankingController);
	if (m_userController)
		SC_UserController_Release(m_userController);
}

void CScores::Render()
{
	if (m_loaded)
	{
		Iw2DSetColour(0xffffffff);
		int32 dw = Iw2DGetSurfaceWidth();
		int32 dh = Iw2DGetSurfaceHeight();

		Iw2DFillRect(CIwSVec2(0, 0), CIwSVec2((int16)dw, (int16)dh));
		for (uint32 i = 0; i < m_guicontrols.size(); i++)
			m_guicontrols[i]->Render();
		if (m_closing)
		{
			int32 clr = 255 * (MAX_CLOSETIME - m_closetime) / MAX_CLOSETIME;
			clr = clr << 24;
			//clr += 0xffffff;
			Iw2DSetColour(clr);
			Iw2DFillRect(CIwSVec2(0, 0), CIwSVec2((int16)dw, (int16)dh));
		}
		if (m_showing)
		{
			int32 clr = 255 * (m_showtime) / MAX_SHOWTIME;
			clr = clr << 24;
		//	clr += 0xffffff;
			Iw2DSetColour(clr);
			Iw2DFillRect(CIwSVec2(0, 0), CIwSVec2((int16)dw, (int16)dh));
		}
	}
}

void CScores::Update(int deltaTimeMs)
{
	CBaseScene::Update(deltaTimeMs);
	if (m_closetime > 0)
	{
		m_closetime -= deltaTimeMs;
		if	(m_closetime <= 0)
		{
			m_closetime = 0;
			g_curSceneType = m_needscenetype;
		}
	}
	else if (m_showtime > 0)
	{
		m_showtime -= deltaTimeMs;
		if (m_showtime <= 0)
		{
			m_showing = false;
			m_showtime = 0;
		}
	}
}

bool CScores::Load()
{
	if (!m_loaded)
	{
		int32 dw = Iw2DGetSurfaceWidth();
		int32 dh = Iw2DGetSurfaceHeight();
		int16 cx, cy, cw, ch;//, dx, dy;
		CGUIImageButton *btn; 
		//CIw2DImage *img;

		cx = 20;
		cy = 20;
		cw = 100;
		ch = g_font->GetHeight();
		btn = new CGUIImageButton(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0, false, g_font, false);
		btn->SetCaption("back");
		m_guicontrols.push_back((CGUIControl *)btn);

		cx = 20;
		cy = 20 + g_font->GetHeight() + 20;
		cw = 100;
		ch = g_font->GetHeight();
		btn = new CGUIImageButton(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0, false, g_font, false);
		btn->SetCaption("profile");
		m_guicontrols.push_back((CGUIControl *)btn);

		cx = 20;
		cy = 20 + g_font->GetHeight() + 20 + g_font->GetHeight() + 20;
		cw = 100;
		ch = g_font->GetHeight();
		btn = new CGUIImageButton(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0, false, g_font, false);
		btn->SetCaption("local");
		m_guicontrols.push_back((CGUIControl *)btn);

		cx = 20;
		cy = 20 + g_font->GetHeight() + 20 + g_font->GetHeight() + 20 + g_font->GetHeight() + 20;
		cw = 100;
		ch = g_font->GetHeight();
		btn = new CGUIImageButton(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0, false, g_font, false);
		btn->SetCaption("global");
		m_guicontrols.push_back((CGUIControl *)btn);

		m_listview = new CGUIListView(CIwSVec2(dw / 3, dh / 5), CIwSVec2(dw * 2 / 3, dh * 4 / 5), g_font, false);
		m_listview->set_needselect(false);
		m_listview->set_needimages(false);
		m_listview->set_itemsubtextsize(m_listview->get_itemsize() / 2);

		m_guicontrols.push_back((CGUIControl *)m_listview);

		m_loaded = true;
	}

	return m_loaded;
}

void CScores::UnLoad()
{
	if (m_loaded)
	{
		CBaseScene::UnLoad();
		m_listview = 0;
	}
	m_loaded = false;
}

void CScores::Show()
{
	CBaseScene::Show();
	FillLocalScoresTable();
}

void CScores::ControlPress(int index)
{
	switch (index)
	{
		case 0:
		{
			m_needscenetype = stMainMenu;
			Close();
		} break;
		case 1:
		{
			m_needscenetype = stProfile;
			Close();
		} break;
		case 2:
		{
			FillLocalScoresTable();
		} break;
		case 3:
		{
			// Loading Scoreloop scores
			// First loading score and rank 
			// See OnRequestSuccessRanking()
			SC_RankingController_Cancel(m_rankingController); //Cancel last request
			SC_User_h user = SC_Session_GetUser(SC_Client_GetSession(g_client));
			SC_RankingController_SetSearchList(m_rankingController, SC_SCORE_SEARCH_LIST_GLOBAL);
			SC_RankingController_SetMode(m_rankingController, 1);
			SC_RankingController_SetTimeInterval(m_rankingController, SC_TIME_INTERVAL_GLOBAL); 
			SC_RankingController_RequestRankingForUser(m_rankingController, user);
			
			StartWait("Loading current user score");
			
		} break;
	}
}

void CScores::DoSubmitScore()
{
	unsigned int aMode = 1;	
	unsigned int aLevel = 1;
	double aScore = (double)g_localhighscore;
	double aMinorResult = (double)s3eDeviceGetInt(S3E_DEVICE_OS); // Save OS type as Minor Result
	
	/* Create the score and set score data */
	SC_Score_h score;
	
	SC_Error_t retCode = SC_Score_New(&score);
	if (retCode == SC_OK) {
		SC_Score_SetResult(score, aScore);
		SC_Score_SetMode(score, aMode);
		SC_Score_SetLevel(score, aLevel);
		SC_Score_SetMinorResult(score, aMinorResult);

		retCode = SC_ScoreController_SubmitScore(m_scoreController, score);
		// See OnRequestSuccessSubmit()
	}
	
	/* SC_ScoreController_SubmitScore takes the ownership, we can safely release it */
	SC_Score_Release(score);
	
	if (retCode != SC_OK) {
		OnRequestFailed(retCode);
	}
}

void CScores::FillLocalScoresTable()
{
	m_listview->Clear();
	char buf[128];
	int index;
	for (int i = 0; i < LOCAL_SCORES_COUNT; i++)
	{
		sprintf(buf, "%d. %s", g_localscores[i].position + 1, g_localscores[i].name);

		index = m_listview->AddItem(NULL, buf);
		m_listview->SetItemCaptionRighti(index, g_localscores[i].score);
	}
}

void CScores::OnRequestSuccess()
{
	m_listview->Clear();
	SC_ScoreList_h scores = SC_ScoresController_GetScores(m_scoresController);
	unsigned int count = SC_ScoreList_GetScoresCount(scores);
	int index;
	char buf[128];
	for (unsigned int i = 0; i < count; i++)
	{
		/* Get the i-th score */
		SC_Score_h score = SC_ScoreList_GetScore(scores, i);
		
		/* Get login of the score's user */
		SC_User_h user = SC_Score_GetUser(score);
		const char* login = SC_String_GetData(SC_User_GetLogin(user));
		sprintf(buf, "%d. %s", (int)SC_Score_GetRank(score), login);
		index = m_listview->AddItem(NULL, buf);
		m_listview->SetItemCaptionRighti(index, (int)SC_Score_GetResult(score));
		//Get OS type from Minor Result
		int OSindex = (int)SC_Score_GetMinorResult(score);
		if (OSindex < 0 && OSindex > 22)
			OSindex = 0;
		m_listview->SetItemSubtext(index, g_OScaptions[OSindex]);
	}
	if (m_curuserrank > 0)
	{
		if (m_curuserrank <= SC_PAGE_SCORES_COUNT && m_curuserrank < (int)m_listview->get_itemscount() + 1)
		{
			CGUIListItem *li = m_listview->get_item(m_curuserrank - 1);
			li->fontcolor = 0xff0000ff;
			m_listview->MoveItemToCenter(m_curuserrank - 1);
			FinishWait();
		}
		else
		{
			// Loading user score if rank > SC_PAGE_SCORES_COUNT
			// See OnRequestSuccessUser()
			SC_UserController_RequestUserDetail(m_userController);
			StartWait("Loading current user details");
		}
	}
	else
	{
		FinishWait();
	}	
	
}

void CScores::OnRequestFailed(SC_Error_t error)
{
	FinishWait();
	ShowErrorDlg("Connection Error");
}

void CScores::ScoresControllerCallback(void *userData, SC_Error_t completionStatus)
{
	CScores* self = static_cast<CScores*> (userData);
	
	if (completionStatus == SC_OK) {		
		self->OnRequestSuccess();
	} 
	else {
		self->OnRequestFailed(completionStatus);
	}
}

void CScores::ScoreControllerCallbackSubmit(void *userData, SC_Error_t completionStatus)
{
	CScores* self = static_cast<CScores*> (userData);
	
	if (completionStatus == SC_OK) 		
		self->OnRequestSuccessSubmit();
	else 
		self->OnRequestFailedSubmit(completionStatus);
}

void CScores::OnRequestSuccessSubmit()
{
	g_localhighscoresubmited = true;
	Save();
	//Loading rank and score again
	//See OnRequestSuccessRanking()
	SC_User_h user = SC_Session_GetUser(SC_Client_GetSession(g_client));
	SC_RankingController_SetSearchList(m_rankingController, SC_SCORE_SEARCH_LIST_GLOBAL);
	SC_RankingController_SetMode(m_rankingController, 1);
	SC_RankingController_SetTimeInterval(m_rankingController, SC_TIME_INTERVAL_GLOBAL); 
	SC_RankingController_RequestRankingForUser(m_rankingController, user);
	StartWait("Loading current user score");		

}
void CScores::OnRequestFailedSubmit(SC_Error_t error)
{
	FinishWait();
	ShowErrorDlg("Connection Error");
}

void CScores::OnRequestSuccessRanking()
{	
	//Get score and rank
	m_curuserscore = 0;
	m_curuserrank = (int)SC_RankingController_GetRanking(m_rankingController);
	SC_Score_h score = SC_RankingController_GetScore(m_rankingController);
	if (score)
	{
		m_curuserscore = (int)SC_Score_GetResult(score);
		m_curuserminorresult = (int)SC_Score_GetMinorResult(score);
	}
	if (g_localhighscore > m_curuserscore) //Submit local highscore if need be
	{
		
		DoSubmitScore();
		StartWait("Submiting current user score");
		
	}
	else // Else loading scores list
	{
		//See OnRequestSuccess()
		SC_ScoresController_SetMode(m_scoresController, 1);
		SC_ScoresController_LoadRange(m_scoresController, 0, SC_PAGE_SCORES_COUNT);		
		StartWait("Loading global scores");
	}
}

void CScores::OnRequestFailedRanking(SC_Error_t error)
{
	FinishWait();
	ShowErrorDlg("Connection Error");
}

void CScores::RankingControllerCallback(void *userData, SC_Error_t completionStatus)
{
	CScores* self = static_cast<CScores*> (userData);
	
	if (completionStatus == SC_OK) {		
		self->OnRequestSuccessRanking();
	} 
	else {
		self->OnRequestFailedRanking(completionStatus);
	}
}

void CScores::OnRequestSuccessUser()
{
	FinishWait();
	SC_User_h user = SC_UserController_GetUser(m_userController);
	SC_String_h login = SC_User_GetLogin(user);
	if (login)
	{
		char buf[128];
		const char* login = SC_String_GetData(SC_User_GetLogin(user));
		sprintf(buf, "%d. %s", m_curuserrank, login);
		int index = m_listview->AddItem(NULL, buf);
		m_listview->SetItemCaptionRighti(index, m_curuserscore);
		//Get OS type from Minor Result
		int OSindex = m_curuserminorresult;
		if (OSindex < 0 && OSindex > 22)
			OSindex = 0;
		m_listview->SetItemSubtext(index, g_OScaptions[OSindex]);
		CGUIListItem *li = m_listview->get_item(index);
		li->fontcolor = 0xff0000ff;
		m_listview->MoveItemToCenter(index);
	}

}

void CScores::OnRequestFailedUser(SC_Error_t error)
{
	FinishWait();
	ShowErrorDlg("Connection error");
}

void CScores::UserControllerCallback(void *userData, SC_Error_t completionStatus)
{
	CScores *self = static_cast<CScores *> (userData);
	
	if (completionStatus == SC_OK) {
		self->OnRequestSuccessUser();
	}
	else {
		self->OnRequestFailedUser(completionStatus);
	}
}