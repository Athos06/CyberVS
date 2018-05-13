#ifndef GUI_SELECTPLAYERS_H
#define GUI_SELECTPLAYERS_H

#include "GUIState.h"

class GUIManager;

class GUISelectPlayers : public Ogre::Singleton<GUISelectPlayers>, GUIState 
{
public:
	
	static GUISelectPlayers& getSingleton();
	static GUISelectPlayers* getSingletonPtr();
	
	GUISelectPlayers();
	~GUISelectPlayers();
	
	DECLARE_GUISTATE_CLASS(GUISelectPlayers)

	void enter();
	void exit();
	void loadCEGUI();
	void resume();


	bool onPVPButton(const CEGUI::EventArgs &args);
	bool onPvsAIButton(const CEGUI::EventArgs &args);
	bool onAIvsAIButton(const CEGUI::EventArgs &args);
	bool onBackButton(const CEGUI::EventArgs &args);

	
private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;
};

#endif
