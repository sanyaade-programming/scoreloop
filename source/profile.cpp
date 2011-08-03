#include "profile.h"
#include "common.h"
#include "IwMaterial.h"
#include "render.h"

CProfile::CProfile(): CBaseScene()
{
	m_keyboard = 0;
	m_scaction = saNone;

	//SC_Error_t retCode = SC_Client_CreateUserController(g_client, &m_userController,
	//										  UserControllerCallback, this);
	SC_Client_CreateUserController(g_client, &m_userController,
											  UserControllerCallback, this);

}

CProfile::~CProfile()
{
	if (m_userController)
		SC_UserController_Release(m_userController);
	UnLoad();
}


void CProfile::Render()
{
	if (m_loaded)
	{
		Iw2DSetColour(0xffffffff);
		int32 dw = Iw2DGetSurfaceWidth();
		int32 dh = Iw2DGetSurfaceHeight();

		Iw2DFillRect(CIwSVec2(0, 0), CIwSVec2((int16)dw, (int16)dh));
		for (uint32 i = 0; i < m_guicontrols.size(); i++)
			m_guicontrols[i]->Render();
		if (m_keyboard->GetVisible())
			m_keyboard->Render();
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

void CProfile::Update(int deltaTimeMs)
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
			m_scaction = saEnterScene;
			StartWait("Loading user details");
			SC_UserController_RequestUserDetail(m_userController);
		}
	}
	if (m_keyboard->GetVisible())
		m_keyboard->Update(deltaTimeMs);
}

bool CProfile::Load()
{
	if (!m_loaded)
	{
		int32 dw = Iw2DGetSurfaceWidth();
		int32 dh = Iw2DGetSurfaceHeight();
		int16 cx, cy, cw, ch, dx, dy;
		CGUIImageButton *btn; 

		cw = 100;
		ch = 50;
		cx = ((int16)dw - cw)/ 2; 
		cy = (int16)dh - ch;	
	
		dx = cw / 20;
		dy = ch / 20;
		btn	= new CGUIImageButton(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0, false, g_font, false);
		btn->SetCaption("Back");
		m_guicontrols.push_back((CGUIControl *)btn);

		cx = 20;
		cy = 20;
		cw = 200;
		ch = g_font->GetHeight();
		m_scusernamelbl = new CGUILabel(CIwSVec2(cx, cy), CIwSVec2(cw, ch), g_font, false);
		m_guicontrols.push_back((CGUIControl *)m_scusernamelbl);

		cx = 230;
		cy = 20;
		cw = 100;
		ch = g_font->GetHeight();
		btn = new CGUIImageButton(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0, false, g_font, false);
		btn->SetCaption("change");
		m_guicontrols.push_back((CGUIControl *)btn);

		cx = 20;
		cy = 100;
		cw = 200;
		ch = g_font->GetHeight();
		m_scuseremaillbl = new CGUILabel(CIwSVec2(cx, cy), CIwSVec2(cw, ch), g_font, false);
		m_guicontrols.push_back((CGUIControl *)m_scuseremaillbl);

		cx = 230;
		cy = 100;
		cw = 100;
		ch = g_font->GetHeight();
		btn = new CGUIImageButton(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0, false, g_font, false);
		btn->SetCaption("change");
		m_guicontrols.push_back((CGUIControl *)btn);

		m_keyboard = new CGUIKeyboard(CIwSVec2(0, dh / 3), CIwSVec2(dw, dh * 2 / 3), 0, g_font);
		m_loaded = true;
	}

	return m_loaded;
}

void CProfile::UnLoad()
{
	if (m_loaded)
	{
		CBaseScene::UnLoad();
		if (m_keyboard)
			delete m_keyboard;
		m_keyboard = 0;
	}
	m_loaded = false;
}

void CProfile::ControlPress(int index)
{
	switch (index)
	{
		case 0:
		{
			m_needscenetype = stScores;
			Close();
		} break;
		case 2:
		{
			//SC_UserController_RequestUserDetail(m_userController);
			m_keyboard->SetString(m_scusernamelbl->GetCaption());
			m_keyboard->Show();
			m_scaction = saChangeLogin;

		} break;
		case 4:
		{
			//SC_UserController_RequestUserDetail(m_userController);
			m_keyboard->SetString(m_scuseremaillbl->GetCaption());
			m_keyboard->Show();
			m_scaction = saChangeEMail;

		} break;
	}
}

void CProfile::PointerDown(int32 x, int32 y)
{
	if (m_keyboard->GetVisible())
	{
		m_keyboard->Down(x, y);
	}
	else
	{
		CBaseScene::PointerDown(x, y);
	}
}

void CProfile::PointerUp(int32 x, int32 y)
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

					SC_User_h user = SC_UserController_GetUser(m_userController);
					switch (m_scaction)
					{
						case saNone: break;
						case saEnterScene: break;
						case saChangeLogin: SC_User_SetLogin(user, m_keyboard->GetString()); break;
						case saChangeEMail: SC_User_SetEmail(user, m_keyboard->GetString()); break;
					}
					SC_UserController_UpdateUser(m_userController);
					StartWait("Saving user details");
				}; break;
			}
		}
	}
	else
	{
		CBaseScene::PointerUp(x, y);
	}
}

void CProfile::PointerMove(int32 x, int32 y)
{
	if (m_keyboard->GetVisible())
	{
		m_keyboard->Move(x, y);
	}
	else
	{
		CBaseScene::PointerMove(x, y);
	}
}

void CProfile::Show()
{
	CBaseScene::Show();

}

void CProfile::OnRequestSuccess()
{
	FinishWait();
	SC_User_h user = SC_UserController_GetUser(m_userController);
	SC_String_h login = SC_User_GetLogin(user);
	SC_String_h email = SC_User_GetEmail(user);
	if (login)
		m_scusernamelbl->SetCaption(SC_String_GetData(login));
	else
		m_scusernamelbl->SetCaption("");

	if (email)
		m_scuseremaillbl->SetCaption(SC_String_GetData(email));
	else
		m_scuseremaillbl->SetCaption("");	  
	//m_scuseremaillbl->SetCaption(SC_String_GetData(email));
	m_scaction = saNone;

}

void CProfile::OnRequestFailed(SC_Error_t error)
{
	FinishWait();
	ShowErrorDlg("Connection error");
	if (m_scaction == saEnterScene)
	{
		m_needscenetype = stScores;
		Close();
	}
	m_scaction = saNone;
}

void CProfile::UserControllerCallback(void *userData, SC_Error_t completionStatus)
{
	CProfile *self = static_cast<CProfile *> (userData);
	
	if (completionStatus == SC_OK) {
		self->OnRequestSuccess();
	}
	else {
		self->OnRequestFailed(completionStatus);
	}
}