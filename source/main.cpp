#include "Iw2D.h"
#include "IwGx.h"
#include "game.h"
#include "basescene.h"
#include "mainmenu.h"
#include "profile.h"
#include "scores.h"
#include "common.h"
#include "s3ePointer.h"
#include "sc_client.h"
#include "IwResManager.h"
#include "render.h"




bool g_AccelerometerEnabled = true;


CGame *game;
CMainMenu *mainmenu;
CProfile *profile;
CScores *scores;

bool g_pointerdown;
int32 g_pointerx;
int32 g_pointery;




enum PointerEventType {petNone, petDown, petUp, petMove};


PointerEventType UpdatePointer()
{
	PointerEventType res = petNone;
	s3ePointerUpdate();
	if (g_pointerdown)
	{
		if (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN)
		{
			int32 x = s3ePointerGetX();
			int32 y = s3ePointerGetY();
			if (x != g_pointerx || y != g_pointery)
			{
				res = petMove;
				g_pointerx = x;
				g_pointery = y;
			}
		}
		else
		{
			g_pointerdown = false;
			res = petUp;
		}
	}
	else
	{
		if (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_DOWN)
		{
			g_pointerx = s3ePointerGetX();
			g_pointery = s3ePointerGetY();
			g_pointerdown = true;
			res = petDown;
		}
	}

	return res;
}

void LoadMainControls()
{
	int16 cx, cy, cw, ch;
	CGUILabel *lbl;
	int32 dw = Iw2DGetSurfaceWidth();
	int32 dh = Iw2DGetSurfaceHeight();

	g_waiting = false;

	cx = (int16)dw / 4;
	cy = (int16)dh / 4;
	cw = (int16)dw / 2;
	ch = (int16)dh / 2;
	g_errorDialog = new CGUIDialog(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0);
	g_errorDialog->SetVisible(false);

	cx = 0;
	cy = 0;
	lbl = new CGUILabel(CIwSVec2(cx, cy), g_errorDialog->get_size(), g_font, false);
	lbl->SetCaption("Error");
	lbl->SetHorzAlign(IW_2D_FONT_ALIGN_CENTRE);
	g_errorDialog->AddControl((CGUIControl *)lbl);
}


void UnloadMainControls()
{
	delete g_errorDialog;
}

int main(int argc, char* argv[])
{
	g_pointerdown = false;
	FillDefaultLocalScores();
	// Initialisation of Airplay Studio modules
	Iw2DInit();			// Initialise support for rendering with the standard SW renderer
	IwGxLightingOff();
	IwResManagerInit();
	Load();

	int16 sw = (int16)Iw2DGetSurfaceWidth();
	int16 sh = (int16)Iw2DGetSurfaceHeight();

	PAL_InitData_t palInitData;
	SC_Error_t retCode = SC_Client_New(&g_client, &palInitData, SC_TEST_GAME_ID,
			SC_TEST_GAME_SECRET, "AIW");

	if (retCode != SC_OK) 
		s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP, "Failed to create SC_Client");
	
	IwGetResManager()->LoadGroup("fonts.group");
	LoadGlobalImages();

	LoadMainControls();
	game = new CGame();
	mainmenu = new CMainMenu();
	profile = new CProfile();
	scores = new CScores();


	mainmenu->Load();
	game->Load();
	profile->Load();
	scores->Load();
	CBaseScene *curscene = (CBaseScene *)mainmenu;
	g_curSceneType = stMainMenu;


	uint32 timer = (uint32)s3eTimerGetMs();

	g_AccelerometerEnabled = (s3eAccelerometerStart() == S3E_RESULT_SUCCESS);

	

	while(curscene)
	{

		s3eDeviceYield(0);


		//// Check for user quit
		if (s3eDeviceCheckQuitRequest())
			break;


		int delta = uint32(s3eTimerGetMs()) - timer;
		timer += delta;

		// Make sure the delta-time value is safe
		if (delta < 0)
			delta = 0;
		if (delta > 100)
			delta = 100;

		if (g_AccelerometerEnabled)
		{

			if (!g_waiting)
			{
				if (g_errorDialog->Get_Visible())
				{
					if (UpdatePointer() == petDown)
						CloseErrorDlg();
					else
						g_errorDialog->Update(delta);
				}
				else
				{
					switch (UpdatePointer())
					{
						case petNone: break;
						case petDown: curscene->PointerDown(g_pointerx, g_pointery); break;
						case petUp: curscene->PointerUp(g_pointerx, g_pointery); break;
						case petMove: curscene->PointerMove(g_pointerx, g_pointery); break;
					}
					curscene->Update(delta);
				}
			}
			curscene->Render();
			if (g_waiting)
			{
				Iw2DSetColour(0xeeffffff);
				Iw2DFillRect(CIwSVec2(0, 0), CIwSVec2(sw, sh));

				Iw2DSetColour(0xff000000);
		
				Iw2DSetFont(g_font);
				Iw2DDrawString(g_waitcaption, CIwSVec2(0, 0), CIwSVec2(sw, sh), IW_2D_FONT_ALIGN_CENTRE, IW_2D_FONT_ALIGN_CENTRE);
			}
			if (g_errorDialog->Get_Visible())
			{
				g_errorDialog->Render();
			}
		}


		Iw2DSurfaceShow();

		switch (g_curSceneType)
		{
			case stNone: curscene = 0; break;
			case stMainMenu: 
			{
				if (curscene != (CBaseScene *)mainmenu)
				{
					curscene = (CBaseScene *)mainmenu; 
					curscene->Show();
				}
			}	break;
			case stGame: 
			{
				if (curscene != (CBaseScene *)game)
				{
					curscene = (CBaseScene *)game;
					curscene->Show();
				}
			}break;

			case stProfile: 
			{
				if (curscene != (CBaseScene *)profile)
				{
					curscene = (CBaseScene *)profile;
					curscene->Show();
				}
			}break;
			case stScores: 
			{
				if (curscene != (CBaseScene *)scores)
				{
					curscene = (CBaseScene *)scores;
					curscene->Show();
				}
			}break;
		}
	}
	Save();
	delete game;
	delete mainmenu;
	delete profile;
	delete scores;

	SC_Client_Release(g_client);
	UnloadGlobalImages();
	UnloadMainControls();
	if (g_AccelerometerEnabled)
		s3eAccelerometerStop();
	// Terminate system modules
	IwResManagerTerminate();
	Iw2DTerminate();

	return 0;
}