#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"


#include "GUIManager.h"
#include "GUIGameOver.h"
#include "GUICredits.h"

#include "GameManager.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUIGameOver* Ogre::Singleton<GUIGameOver>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUIGameOver& GUIGameOver::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIGameOver* GUIGameOver::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}

GUIGameOver::GUIGameOver(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIGameOver::~GUIGameOver(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIGameOver::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();

	showWinner();

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIGameOver::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIGameOver;	 
}
//|||||||||||||||||||||||||||||||||||||||||||||||
void GUIGameOver::showWinner(){
	int winner = GameManager::getSingletonPtr()->getWinner();
	
	CEGUI::Window * WinnerText = CEGUI::WindowManager::getSingleton().
				getWindow("GameOverRoot/bg/score");
	CEGUI::Window* WinnerImg1 = CEGUI::WindowManager::getSingleton().
		getWindow("GameOverRoot/bg/P1wins");
	CEGUI::Window* WinnerImg2 = CEGUI::WindowManager::getSingleton().
		getWindow("GameOverRoot/bg/P2wins");
	
	WinnerImg1->hide();
	WinnerImg2->hide();

	WinnerText->setText(Ogre::StringConverter::toString(GameManager::getSingletonPtr()
		->getScore()[0])+"-"+Ogre::StringConverter::toString(GameManager::getSingletonPtr()
		->getScore()[1]) );
				

	switch(winner){
		case 0:
			WinnerImg1->show();
			break;
		case 1:
			WinnerImg2->show();
			break;
		default:
			break;
	}
}


void GUIGameOver::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
}

void GUIGameOver::resume(){
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();

}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUIGameOver::onPlayAgainButton(const CEGUI::EventArgs &args){
	
	GameManager::getSingletonPtr()->restartGame();
	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		popAppState();
	
	//popGUIState();
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUIGameOver::onBackButton(const CEGUI::EventArgs &args){
	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		popAllAndPushAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("MenuState"));
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

