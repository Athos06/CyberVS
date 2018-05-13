#ifndef GUI_CREDITS_H
#define GUI_CREDITS_H

#include "GUIState.h"

class GUIManager;

class GUICredits : public Ogre::Singleton<GUICredits>, GUIState 
{
public:
	
	static GUICredits& getSingleton();
	static GUICredits* getSingletonPtr();
	
	GUICredits();
	~GUICredits();
	
	DECLARE_GUISTATE_CLASS(GUICredits)

	void enter();
	void exit();
	void loadCEGUI();
	
	bool onBackButton(const CEGUI::EventArgs &args);
	

	
private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;
};

#endif
