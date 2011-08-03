#ifndef _MAINMENU_H
#define _MAINMENU_H

#include "basescene.h"
#include "Iw2D.h"
#include "IwResManager.h"
#include "IwTexture.h"

class CMainMenu: CBaseScene
{
protected:
	void ControlPress(int index);
public:
	CMainMenu();
	~CMainMenu();

	void Render();
	void Update(int deltaTimeMs);
	bool Load();
	void UnLoad();
};


#endif // _MAINMENU_H