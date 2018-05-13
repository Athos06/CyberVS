#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"


#include "GUIManager.h"
#include "GUISelectPlayers.h"
#include "GameManager.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUISelectPlayers* Ogre::Singleton<GUISelectPlayers>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUISelectPlayers& GUISelectPlayers::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUISelectPlayers* GUISelectPlayers::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUISelectPlayers::GUISelectPlayers(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUISelectPlayers::~GUISelectPlayers(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUISelectPlayers::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUISelectPlayers::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetSelectPlayers;	 
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUISelectPlayers::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUISelectPlayers::resume(){
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUISelectPlayers::onPVPButton(const CEGUI::EventArgs &args){
	//se indica que la AI esta desactivada para ambos jugadores
	GameConfig::getSingletonPtr()->AIon[0] = 0;
	GameConfig::getSingletonPtr()->AIon[1] = 0;
	
	pushGUIState(findByName("GUISelectRounds"));
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUISelectPlayers::onPvsAIButton(const CEGUI::EventArgs &args){
	//se indica que la AI esta activada para el jugador 2
	GameConfig::getSingletonPtr()->AIon[0] = 0;
	GameConfig::getSingletonPtr()->AIon[1] = 1;
	
	pushGUIState(findByName("GUISelectRounds"));
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUISelectPlayers::onAIvsAIButton(const CEGUI::EventArgs &args){
	//se indica que la AI esta activada para el jugador 1
	GameConfig::getSingletonPtr()->AIon[0] = 1;
	GameConfig::getSingletonPtr()->AIon[1] = 1;
	
	pushGUIState(findByName("GUISelectRounds"));
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUISelectPlayers::onBackButton(const CEGUI::EventArgs &args){
	popGUIState();
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||