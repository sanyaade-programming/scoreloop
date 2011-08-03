#include "mainmenu.h"
#include "common.h"
#include "s3eAccelerometer.h"
#include "IwMaterial.h"
#include "render.h"

CIwVec2 vec;

CMainMenu::CMainMenu(): CBaseScene()
{
	vec = CIwVec2(0, 0);
}

CMainMenu::~CMainMenu()
{
	UnLoad();
}


void CMainMenu::Render()
{
	if (m_loaded)
	{
		int16 dw = (int16)Iw2DGetSurfaceWidth();
		int16 dh = (int16)Iw2DGetSurfaceHeight();
		Iw2DSetColour(0xff000000);
		Iw2DFillRect(CIwSVec2(0, 0), CIwSVec2(dw, dh));
		Iw2DSetColour(0xffffffff);

      // Allocate a material from the IwGx global cache
        CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
        pMat->SetModulateMode(CIwMaterial::MODULATE_NONE);
	
		for (uint32 i = 0; i < m_guicontrols.size(); i++)
			m_guicontrols[i]->Render();
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


void CMainMenu::Update(int deltaTimeMs)
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
		}
	}

}

void CMainMenu::ControlPress(int index)
{
	switch (index)
	{
		case 0:
		{
			m_needscenetype = stGame;
			Close();
		}
		break;
		case 1:
		{
			//m_needscenetype = stInfo;
			m_needscenetype = stScores;
			
			Close();
		}
		break;
	}
}


bool CMainMenu::Load()
{
	if (!m_loaded)
	{
		int16 dw = (int16)Iw2DGetSurfaceWidth();
		int16 dh = (int16)Iw2DGetSurfaceHeight();
		int16 cx, cy, cw, ch;
		CGUIImageButton *btn;

	
		cw = 100;
		ch = 50;
		cx = 100;
		cy = 100;
		btn	= new CGUIImageButton(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0, false, g_font, false);
		btn->SetCaption("play");
		m_guicontrols.push_back((CGUIControl *)btn);

		cw = 100 ;
		ch = 50;
		cx = 100;
		cy = 200;	
		btn	= new CGUIImageButton(CIwSVec2(cx, cy), CIwSVec2(cw, ch), 0, false, g_font, false);
		btn->SetCaption("scores");
		m_guicontrols.push_back((CGUIControl *)btn);
		
		m_loaded = true;
	}

	return m_loaded;
}


void CMainMenu::UnLoad()
{
	if (m_loaded)
	{
		CBaseScene::UnLoad();
	}
	m_loaded = false;
}


