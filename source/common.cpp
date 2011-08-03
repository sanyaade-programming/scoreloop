#include "common.h"
#include "s3eFile.h"
#include "string.h"
#include "stdio.h"

#define FILENAME "main.sav"

SceneType g_curSceneType;
int g_levelindex;
bool g_localhighscoresubmited;
int g_localhighscore;
char g_localname[128];
CScoreLine g_localscores[LOCAL_SCORES_COUNT];
bool g_waiting;
char g_waitcaption[128];
CGUIDialog *g_errorDialog;

SC_Client_h g_client;


bool Save()
{
	//Saving local scores to file
	int16 ver = 1;
	s3eFile* fh = s3eFileOpen(FILENAME, "w");
	bool res = fh != NULL;
	 

	if (res)
    {
		res = (s3eFileWrite(&ver, sizeof(ver), 1, fh) == 1) &&	
			  (s3eFileWrite(&g_localhighscore, sizeof(g_localhighscore), 1, fh) == 1) && 
			  (s3eFileWrite(&g_localname, sizeof(g_localname), 1, fh) == 1) && 
			  (s3eFileWrite(&g_localscores, sizeof(g_localscores), 1, fh) == 1) &&
			  (s3eFileWrite(&g_localhighscoresubmited, sizeof(g_localhighscoresubmited), 1, fh) == 1);
		s3eFileClose(fh);
    }
	return res;
}

bool Load()
{
	//Loading local scores from file
	g_localhighscoresubmited = true;
	g_localhighscore = 0;
	s3eFile* fh = s3eFileOpen(FILENAME, "r");
	bool res = fh != NULL;
	 
	if (res)
    {
		int16 ver;
		res = s3eFileRead(&ver, sizeof(ver), 1, fh) == 1;
		if (res)
		{
			switch (ver)
			{
				case 1:
				{
					res = (s3eFileRead(&g_localhighscore, sizeof(g_localhighscore), 1, fh) == 1) &&
						  (s3eFileRead(&g_localname, sizeof(g_localname), 1, fh) == 1) &&
						  (s3eFileRead(&g_localscores, sizeof(g_localscores), 1, fh) == 1) &&
						  (s3eFileRead(&g_localhighscoresubmited, sizeof(g_localhighscoresubmited), 1, fh) == 1);
				}
				break;
			}
		}
		s3eFileClose(fh);
    }
	return res;
}

void FillDefaultLocalScores()
{
	//Default local scores
	memset(g_localscores, 0, sizeof(g_localscores));
	g_localhighscore = 0;
	sprintf(g_localname, "PLAYER");
	
	g_localscores[0].position = 0;
	sprintf(g_localscores[0].name, "PLAYER_0");
	g_localscores[0].score = 25000;

	g_localscores[1].position = 1;
	sprintf(g_localscores[1].name, "PLAYER_1");
	g_localscores[1].score = 20000;

	g_localscores[2].position = 2;
	sprintf(g_localscores[2].name, "PLAYER_2");
	g_localscores[2].score = 17000;

	g_localscores[3].position = 3;
	sprintf(g_localscores[3].name, "PLAYER_3");
	g_localscores[3].score = 15000;

	g_localscores[4].position = 4;
	sprintf(g_localscores[4].name, "PLAYER_4");
	g_localscores[4].score = 12000;

	g_localscores[5].position = 5;
	sprintf(g_localscores[5].name, "PLAYER_5");
	g_localscores[5].score = 10000;

	g_localscores[6].position = 6;
	sprintf(g_localscores[6].name, "PLAYER_6");
	g_localscores[6].score = 8000;

	g_localscores[7].position = 7;
	sprintf(g_localscores[7].name, "PLAYER_7");
	g_localscores[7].score = 7000;

	g_localscores[8].position = 8;
	sprintf(g_localscores[8].name, "PLAYER_8");
	g_localscores[8].score = 6000;

	g_localscores[9].position = 9;
	sprintf(g_localscores[9].name, "PLAYER_9");
	g_localscores[9].score = 5000;

	
}

void SortScores()
{
	bool b = true;
	while (b)
	{
		b = false;
		for (int i = 0; i < LOCAL_SCORES_COUNT - 1; i++)
		{
			if (g_localscores[i].score < g_localscores[i + 1].score)
			{
				CScoreLine temp = g_localscores[i];
				g_localscores[i] = g_localscores[i + 1];
				g_localscores[i + 1] = temp;
				g_localscores[i].position = i;
				g_localscores[i + 1].position = i + 1;
				b = true;

			}
		}
	}
}

void SubmitLocalScore(int value)
{
	//put score to the end and sort
	if (value > g_localscores[LOCAL_SCORES_COUNT - 1].score)
	{
		sprintf(g_localscores[LOCAL_SCORES_COUNT - 1].name, g_localname);
		g_localscores[LOCAL_SCORES_COUNT - 1].score = value;
		SortScores();
		Save();
	}
}

int GetLocalScorePosition(int score)
{
	//Finding rank of local score
	int res = LOCAL_SCORES_COUNT;
	while (res > 0 && g_localscores[res - 1].score < score) res--;
	return res;
}



void StartWait(const char *caption)
{
	g_waiting = true;
	if (caption)
		sprintf(g_waitcaption, caption);
	else
		g_waitcaption[0] = '\0';
}

void FinishWait()
{
	g_waiting = false;
}

void ShowErrorDlg(const char *text)
{
	((CGUILabel *)g_errorDialog->GetControl(0))->SetCaption(text);
	g_errorDialog->Show();
}

void CloseErrorDlg()
{
	g_errorDialog->Close();
}