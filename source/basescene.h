#ifndef _BASESCENE_H
#define _BASESCENE_H


#include "gui.h"
#include "IwUtil.h"
#include "common.h"

const int MAX_SHOWTIME = 250;
const int MAX_CLOSETIME = 250;

class CBaseScene //Base scene class
{
protected:
	bool m_loaded;
	int m_showtime;
	int m_closetime;
	bool m_showing;
	bool m_closing;
	int m_downcontrolindex;
	SceneType m_needscenetype;
	CIwArray<CGUIControl *> m_guicontrols;

	virtual void ControlPress(int index){};
	virtual void ControlDown(int index){};
	virtual void ControlUp(int index){};
public:
	CBaseScene(){m_loaded = false; m_showtime = 0; m_closetime = 0; m_showing = false; m_closing = false;};
	~CBaseScene(){};

	virtual void Render(){};
	virtual void Update(int deltaTimeMs)
	{		
		for (uint32 i = 0; i < m_guicontrols.size(); i++)
			m_guicontrols[i]->Update(deltaTimeMs);
	};
	virtual bool Load(){return true;};
	virtual void UnLoad()
	{
		for (uint32 i = 0; i < m_guicontrols.size(); i++)
			delete m_guicontrols[i];
		m_guicontrols.clear();
	};

	int GetGUIControl(CIwSVec2 pos)
	{
		int res = -1;
		for (uint32 i = 0; res == -1 && i < m_guicontrols.size(); i++)
		{
			if (pos.x > m_guicontrols[i]->get_topLeft().x &&
				pos.x < m_guicontrols[i]->get_topLeft().x + m_guicontrols[i]->get_size().x &&
				pos.y > m_guicontrols[i]->get_topLeft().y &&
				pos.y < m_guicontrols[i]->get_topLeft().y + m_guicontrols[i]->get_size().y)
				res = i;
		}
		return res;
	}
	virtual void Show(){m_showtime = MAX_SHOWTIME; m_closetime = 0; m_showing = true; m_closing = false;};
	virtual void Close(){m_showtime = 0; m_closetime = MAX_CLOSETIME; m_showing = false; m_closing = true;};

	virtual void PointerDown(int32 x, int32 y)
	{
		m_downcontrolindex = -1;
		for (uint32 i = 0; m_downcontrolindex == -1 && i < m_guicontrols.size(); i++)
		{
			if (m_guicontrols[i]->Down(x, y))
			{
				ControlDown(i);
				m_downcontrolindex = i;
			}
		}
	};
	virtual void PointerUp(int32 x, int32 y)
	{
		if (m_downcontrolindex >= 0 && (m_guicontrols[m_downcontrolindex]->Up(x, y) || m_guicontrols[m_downcontrolindex]->GetCapture()))
		{
			ControlPress(m_downcontrolindex);
			ControlUp(m_downcontrolindex);
			m_downcontrolindex = -1;
		}
	};
	virtual void PointerMove(int32 x, int32 y)
	{
		if (m_downcontrolindex >= 0)
		{
			
			m_guicontrols[m_downcontrolindex]->Move(x, y);
		}
	};

};



#endif //_BASESCENE_H