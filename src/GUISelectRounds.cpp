#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"


#include "GUIManager.h"
#include "GUISelectRounds.h"
#include "GameManager.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUISelectRounds* Ogre::Singleton<GUISelectRounds>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUISelectRounds& GUISelectRounds::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUISelectRounds* GUISelectRounds::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUISelectRounds::GUISelectRounds(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUISelectRounds::~GUISelectRounds(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUISelectRounds::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();

	CEGUI::Window * AI1 = CEGUI::WindowManager::getSingleton().
				getWindow("SelectRoundsRoot/StaticImage/AI1");
	CEGUI::Window* AI2 = CEGUI::WindowManager::getSingleton().
				getWindow("SelectRoundsRoot/StaticImage/AI2");
	
	AI1->setEnabled(false);
	AI2->setEnabled(false);
	
	if(GameConfig::getSingletonPtr()->AIon[0])
		AI1->setEnabled(true);
	
	if(GameConfig::getSingletonPtr()->AIon[1])
		AI2->setEnabled(true);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUISelectRounds::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetSelectRounds;	 
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUISelectRounds::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUISelectRounds::resume(){
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUISelectRounds::onPlayButton(const CEGUI::EventArgs &args){
	CEGUI::Spinner* rounds = static_cast<CEGUI::Spinner*>
		(CEGUI::WindowManager::getSingleton().getWindow
		("SelectRoundsRoot/StaticImage/Spinner") );
	int roundsLimit = rounds->getCurrentValue();
	
	GameConfig::getSingletonPtr()->limitScore = roundsLimit;

	int test; 
	if(GameConfig::getSingletonPtr()->AIon[0]){
		CEGUI::RadioButton *radioButtonGroup =static_cast<CEGUI::RadioButton*>
			(CEGUI::WindowManager::getSingleton().getWindow
			("SelectRoundsRoot/StaticImage/AI1/AI1normal") );
		CEGUI::RadioButton * radioButtonSelected =   radioButtonGroup
			->getSelectedButtonInGroup();
	
		GameConfig::getSingletonPtr()->AIdifficulty[0] =
			radioButtonSelected->getID();
	}

	if(GameConfig::getSingletonPtr()->AIon[1]){
		CEGUI::RadioButton *radioButtonGroup2 =static_cast<CEGUI::RadioButton*>
			(CEGUI::WindowManager::getSingleton().getWindow
			("SelectRoundsRoot/StaticImage/AI2/AI2normal") );
		CEGUI::RadioButton *radioButtonSelected2 =   radioButtonGroup2
			->getSelectedButtonInGroup();

		GameConfig::getSingletonPtr()->AIdifficulty[1] =
			radioButtonSelected2->getID();
	}

	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		changeAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("GameState"));

	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUISelectRounds::onBackButton(const CEGUI::EventArgs &args){
	popGUIState();
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||