#ifndef GUI_SELECTROUNDS_H
#define GUI_SELECTROUNDS_H

#include "GUIState.h"

class GUIManager;

class GUISelectRounds : public Ogre::Singleton<GUISelectRounds>, GUIState 
{
public:
	
	static GUISelectRounds& getSingleton();
	static GUISelectRounds* getSingletonPtr();
	
	GUISelectRounds();
	~GUISelectRounds();
	
	DECLARE_GUISTATE_CLASS(GUISelectRounds)

	void enter();
	void exit();
	void loadCEGUI();
	void resume();


	bool onPlayButton(const CEGUI::EventArgs &args);
	bool onBackButton(const CEGUI::EventArgs &args);
	

	
private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;
};

#endif
