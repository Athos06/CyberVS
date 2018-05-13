#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"

#include "GUICredits.h"
#include "GUIManager.h"


template<> GUICredits* Ogre::Singleton<GUICredits>::msSingleton = 0;

GUICredits& GUICredits::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
 
GUICredits* GUICredits::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}


GUICredits::GUICredits(){}

GUICredits::~GUICredits(){
	
}
void GUICredits::enter()
{
	
	loadCEGUI();
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();
	
	
}
void GUICredits::loadCEGUI()
{

	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetCredits;

	 
}
void GUICredits::exit()
{
	m_pGUImgr->m_pGUISystem->setGUISheet(0);
}

bool GUICredits::onBackButton(const CEGUI::EventArgs &args)
{
	popGUIState();
	return true;
}