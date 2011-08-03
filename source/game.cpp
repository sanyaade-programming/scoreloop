#include "game.h"


CGame::CGame(): CBaseScene()
{
	m_score = 0;
}

CGame::~CGame()
{
	UnLoad();
}


void CGame::Render()
{
	if (m_loaded)
	{
		int16 dw = (int16)Iw2DGetSurfaceWidth();
		int16 dh = (int16)Iw2DGetSurfaceHeight();
		Iw2DSetColour(0xff000000);
		Iw2DFillRect(CIwSVec2(0, 0), CIwSVec2(dw, dh));
		Iw2DSetColour(0xffffffff);

	
		for (uint32 i = 0; i < m_guicontrols.size(); i++)
			m_guicontrols[i]->Render();

		if (m_lvlFinishDialog->Get_Visible())
			m_lvlFinishDialog->Render();

		if (m_keyboard->GetVisible())
			m_keyboard->Render();

		if (m_closing)
		{
			int32 clr = 255 * (MAX_CLOSETIME - m_closetime) / MAX_CLOSETIME;
			clr = clr << 24;
			//clr += 0xffffff;
			Iw2DSetColour(clr);
			Iw2DFillRect(CIwSVec2(0, 0), CIwSVec2(dw, dh));
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


void CGame::Update(int deltaTimeMs)
{
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
			GenerateRandomScore();
		}
	}
	else if (m_keyboard->GetVisible())
	{
		m_keyboard->Update(deltaTimeMs);
	}
	else if (m_lvlFinishDialog->Get_Visible())
	{
		m_lvlFinishDialog->Update(deltaTimeMs);
	}
}

void CGame::ControlPress(int index)
{
	switch (index)
	{
		case 1:
		{
			GenerateRandomScore();
		}
		break;
		case 2:
		{
			int localpos = GetLocalScorePosition(m_score);
			if (localpos < LOCAL_SCORES_COUNT)
			{
				char buf[64];
				sprintf(buf, "%s %d", "Highscore #", localpos + 1);
				((CGUILabel *)(m_lvlFinishDialog->GetControl(2)))->SetCaption(buf);
			}
			else
			{
				((CGUILabel *)(m_lvlFinishDialog->GetControl(2)))->SetCaption("Score");
			}
			((CGUILabel *)(m_lvlFinishDialog->GetControl(3)))->SetCaptioni(m_score);
			((CGUILabel *)(m_lvlFinishDialog->GetControl(4)))->SetCaption(g_localname);

			if (m_score > g_localhighscore)
			{
				g_localhighscore = m_score;
				g_localhighscoresubmited = false;
				Save();
			}	

			m_lvlFinishDialog->Show();
		}
		break;
	}
}

void CGame::PointerDown(int32 x, int32 y)
{

	if (m_keyboard->GetVisible())
	{
		m_keyboard->Down(x, y);
	}

	else if (m_lvlFinishDialog->Get_Visible())
	{
		m_lvlFinishDialog->Down(x, y);
	}
	else
	{
		CBaseScene::PointerDown(x, y);
	}
}

void CGame::PointerUp(int32 x, int32 y)
{
	if (m_keyboard->GetVisible())
	{
		if (m_keyboard->Up(x, y))
		{
			switch (m_keyboard->Get_ModalRes())
			{
				case -1:
				{
					m_keyboard->Close();
				}; break;
				case 1:
				{
					m_keyboard->Close();
					memset(g_localname, 0, sizeof(g_localname));
					sprintf(g_localname, m_keyboard->GetString());
					((CGUILabel *)(m_lvlFinishDialog->GetControl(4)))->SetCaption(g_localname);
					Save();

				}; break;
			}
		}
	}

	else if (m_lvlFinishDialog->Get_Visible())
	{
		if (m_lvlFinishDialog->Up(x, y))
		{
			switch (m_lvlFinishDialog->Get_ModalRes())
			{
				case 0: 
				{
					if (GetLocalScorePosition(m_score) < LOCAL_SCORES_COUNT)
					{
						SubmitLocalScore(m_score);
						Save();
					}
					m_lvlFinishDialog->Close();
					m_needscenetype = stMainMenu;
					Close();
				}
				break;


				case 1: 
				{
					if (GetLocalScorePosition(m_score) < LOCAL_SCORES_COUNT)
					{
						SubmitLocalScore(m_score);
						Save();
					}
					m_lvlFinishDialog->Close();
				}
				break;
				case 5: 
				{
					m_keyboard->SetString(g_localname);
					m_keyboard->Show();
				}
				break;
			}
		}
	}
	else
	{
		CBaseScene::PointerUp(x, y);
	}
}

void CGame::PointerMove(int32 x, int32 y)
{
	if (m_keyboard->GetVisible())
	{
		m_keyboard->Move(x, y);
	}

	else if (m_lvlFinishDialog->Get_Visible())
	{
		m_lvlFinishDialog->Move(x, y);
	}
	else
	{
		CBaseScene::PointerMove(x, y);

	}
}


bool CGame::Load()
{
	if (!m_loaded)
	{
		int32 dw = Iw2DGetSurfaceWidth();
		int32 dh = Iw2DGetSurfaceHeight();

		int16 cx, cy, cw, ch;
		CGUILabel *lbl;
		CGUIImageButton *btn;
		//Creating random score label
		cx = 100;
		cy = 50;
		cw = 100;
		ch = 50;
		lbl = new CGUILabel(CIwSVec2(cx, cy), CIwSVec2(cw, ch), g_font, false);
		lbl->SetCaptioni(0);
		lbl->SetFontColor(0xffffffff);
		lbl->SetVertAlign(IW_2D_FONT_ALIGN_CENTRE);
		lbl->SetHorzAlign(IW_2D_FONT_ALIGN_CENTRE);
		m_guicontrols.push_back((CGUIControl *)lbl);

		cx = 200;
		cy = 50;
		cw = 100;
		ch = 50;
		btn = new CGUIImageButton(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0, false, g_font, false);
		btn->SetCaption("Random Score");
		m_guicontrols.push_back((CGUIControl *)btn);

		cx = 200;
		cy = 150;
		cw = 100;
		ch = 50;
		btn = new CGUIImageButton(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0, false, g_font, false);
		btn->SetCaption("Finish Game");
		m_guicontrols.push_back((CGUIControl *)btn);


		//Creating virtual keyboard
		m_keyboard = new CGUIKeyboard(CIwSVec2(0, dh / 3), CIwSVec2(dw, dh * 2 / 3), 0, g_font);	


		//Creating finish dialog with buttons and labels
		cx = (int16)dw / 4;
		cy = (int16)dh / 4;
		cw = (int16)dw / 2;
		ch = (int16)dh / 2;
		m_lvlFinishDialog = new CGUIDialog(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0);

		cx = 0;
		cy = m_lvlFinishDialog->get_size().y * 3 / 4;
		cw = m_lvlFinishDialog->get_size().x / 2;
		ch = m_lvlFinishDialog->get_size().y / 4;
		btn = new CGUIImageButton(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0, false, g_font, false);
		btn->SetCaption("Menu");
		m_lvlFinishDialog->AddControl((CGUIControl *)btn);

		cx = m_lvlFinishDialog->get_size().x / 2;
		cy = m_lvlFinishDialog->get_size().y * 3 / 4;
		cw = m_lvlFinishDialog->get_size().x / 2;
		ch = m_lvlFinishDialog->get_size().y / 4;
		btn = new CGUIImageButton(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0, false, g_font, false);
		btn->SetCaption("Replay");
		m_lvlFinishDialog->AddControl((CGUIControl *)btn);

		cx = 0;
		cy = 0;
		cw = m_lvlFinishDialog->get_size().x;
		ch = m_lvlFinishDialog->get_size().y / 4;
		lbl = new CGUILabel(CIwSVec2(cx, cy), CIwSVec2(cw, ch), g_font, false);
		lbl->SetCaption("Title");
		lbl->SetFontColor(0xff666666);
		lbl->SetVertAlign(IW_2D_FONT_ALIGN_CENTRE);
		lbl->SetHorzAlign(IW_2D_FONT_ALIGN_CENTRE);
		m_lvlFinishDialog->AddControl((CGUIControl *)lbl);

		cx = 0;
		cy = m_lvlFinishDialog->get_size().y / 4;
		cw = m_lvlFinishDialog->get_size().x;
		ch = m_lvlFinishDialog->get_size().y / 4;
		lbl = new CGUILabel(CIwSVec2(cx, cy), CIwSVec2(cw, ch), g_font, false);
		lbl->SetCaptioni(0);
		lbl->SetFontColor(0xff666666);
		lbl->SetVertAlign(IW_2D_FONT_ALIGN_CENTRE);
		lbl->SetHorzAlign(IW_2D_FONT_ALIGN_CENTRE);
		m_lvlFinishDialog->AddControl((CGUIControl *)lbl);

		cx = 0;
		cy = m_lvlFinishDialog->get_size().y / 2;
		cw = m_lvlFinishDialog->get_size().x / 2;
		ch = m_lvlFinishDialog->get_size().y / 4;
		lbl = new CGUILabel(CIwSVec2(cx, cy), CIwSVec2(cw, ch), g_font, false);
		lbl->SetCaption("Your name: Player");
		lbl->SetFontColor(0xff666666);
		lbl->SetVertAlign(IW_2D_FONT_ALIGN_CENTRE);
		lbl->SetHorzAlign(IW_2D_FONT_ALIGN_CENTRE);
		m_lvlFinishDialog->AddControl((CGUIControl *)lbl);

		cx = m_lvlFinishDialog->get_size().x / 2;
		cy = m_lvlFinishDialog->get_size().y / 2;
		cw = m_lvlFinishDialog->get_size().x / 2;
		ch = m_lvlFinishDialog->get_size().y / 4;
		btn = new CGUIImageButton(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0, false, g_font, false);
		btn->SetCaption("Change");
		m_lvlFinishDialog->AddControl((CGUIControl *)btn);
		m_loaded = true;
	}

	return m_loaded;
}


void CGame::UnLoad()
{
	if (m_loaded)
	{
		delete m_keyboard;
		delete m_lvlFinishDialog;
		CBaseScene::UnLoad();
	}
	m_loaded = false;
}


void CGame::GenerateRandomScore()
{
	IwRandSeed((int32)s3eTimerGetMs());
	m_score = IwRand();
	((CGUILabel *)m_guicontrols[0])->SetCaptioni(m_score);
}