#ifndef _PROFILE_H
#define _PROFILE_H

#include "basescene.h"
#include "IwResManager.h"
#include "IwTexture.h"


enum SCAction { saNone, saEnterScene, saChangeLogin, saChangeEMail};

class CProfile: public CBaseScene
{
private:
	CGUIKeyboard *m_keyboard;
	CGUILabel *m_scusernamelbl;
	CGUILabel *m_scuseremaillbl;
	SC_UserController_h m_userController;

	SCAction m_scaction;

	void OnRequestSuccess();
	void OnRequestFailed(SC_Error_t error);
	static void UserControllerCallback(void *userData, SC_Error_t completionStatus);
protected:
	void ControlPress(int index);
public:
	CProfile();
	~CProfile();
	void Render();
	void Update(int deltaTimeMs);
	bool Load();
	void UnLoad();
	void Show();

	void PointerDown(int32 x, int32 y);
	void PointerUp(int32 x, int32 y);
	void PointerMove(int32 x, int32 y);
};


#endif //_PROFILE_H