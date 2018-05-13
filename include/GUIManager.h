#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <CEGUI.h>
#include <Ogre.h>

class GUIStateManager;
class GUIMainMenu;
class GUICredits;
class GUIGame;
class GUIPauseMenu;
class GUIGameOver;
class GUIOptions;
class GUIAudio;
class GUIControls;
class GUISelectPlayers;
class GUISelectRounds;

class GUIManager : public Ogre::Singleton<GUIManager>{
public:
	GUIManager();
	~GUIManager();
	
	static GUIManager& getSingleton();
	static GUIManager* getSingletonPtr();

	void initGUI();
	void createGUI();
	void CreateMainMenuGUI();
	void CreateCreditsGUI();
	void CreatePauseMenuGUI();
	void CreateGameOverGUI();
	void CreateOptionsGUI();
	void CreateAudioGUI();
	void CreateControlsGUI();
	void CreateInGameGUI();
	void CreateSelectPlayersGUI();
	void CreateSelectRoundsGUI();

	CEGUI::OgreRenderer*		m_pGUIRenderer;
	CEGUI::System*				m_pGUISystem;
	//probablemente seria mejor opcion incluir todo esto en cada 
	//su GUIstate correspondiente para mayor claridad
	CEGUI::Window*				m_pGUIsheetMainMenu;
	CEGUI::Window*				m_pGUIsheetInGame;
	CEGUI::Window*				m_pGUIsheetCredits;
	CEGUI::Window*				m_pGUIsheetPauseMenu;
	CEGUI::Window*				m_pGUIGameOver;
	CEGUI::Window*				m_pGUIsheetOptions;
	CEGUI::Window*				m_pGUIsheetAudio;
	CEGUI::Window*				m_pGUIsheetControls;
	CEGUI::Window*				m_pGUIsheetSelectPlayers;
	CEGUI::Window*				m_pGUIsheetSelectRounds;

	GUIStateManager* m_pGUIStateManager;

private:

};

#endif