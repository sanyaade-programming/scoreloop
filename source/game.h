#ifndef _GAME_H
#define _GAME_H

#include "IwGeom.h"
#include "s3eAccelerometer.h"
#include "basescene.h"
#include "render.h"



class CGame: CBaseScene
{
private:
	CGUILabel *m_scorelabel;
	CGUIImageButton *m_scorebutton;
	CGUIImageButton *m_finishbutton;
	CGUIDialog *m_lvlFinishDialog;
	CGUIKeyboard *m_keyboard;

	int m_score;
protected:
	void ControlPress(int index);
public:
	CGame();
	~CGame();

	void Render();
	void Update(int deltaTimeMs);
	bool Load();
	void UnLoad();

	void PointerDown(int32 x, int32 y);
	void PointerUp(int32 x, int32 y);
	void PointerMove(int32 x, int32 y);

	void GenerateRandomScore();
};




#endif //_GAME_H