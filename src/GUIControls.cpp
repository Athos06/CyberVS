#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"


#include "GUIManager.h"
#include "GUIControls.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUIControls* Ogre::Singleton<GUIControls>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUIControls& GUIControls::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIControls* GUIControls::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIControls::GUIControls(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIControls::~GUIControls(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIControls::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIControls::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetControls;	 
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIControls::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
}

void GUIControls::resume(){
	//m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUIControls::onBackButton(const CEGUI::EventArgs &args){
	popGUIState();
	return true;
}