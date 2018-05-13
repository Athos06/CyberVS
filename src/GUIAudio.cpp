#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"


#include "GUIManager.h"
#include "GUIAudio.h"
#include "GameManager.h"
#include "AudioManager.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUIAudio* Ogre::Singleton<GUIAudio>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUIAudio& GUIAudio::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIAudio* GUIAudio::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIAudio::GUIAudio(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIAudio::~GUIAudio(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIAudio::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIAudio::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetAudio;	 
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIAudio::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
}

void GUIAudio::resume(){
	//m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUIAudio::onBackButton(const CEGUI::EventArgs &args){
	popGUIState();
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUIAudio::volumeChange(const CEGUI::EventArgs &args){
	CEGUI::Spinner* vol = static_cast<CEGUI::Spinner*>
		(CEGUI::WindowManager::getSingleton().getWindow("AudioRoot/VolumeControl") );
	int musicVol = vol->getCurrentValue();
	
	GameConfig::getSingletonPtr()->volume = musicVol;

	AudioManager::getSingletonPtr()->m_pMainTrack->volume(musicVol);
	return true;
}