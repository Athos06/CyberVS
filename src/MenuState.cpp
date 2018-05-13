//|||||||||||||||||||||||||||||||||||||||||||||||
#include <CEGUI.h>

#include "MenuState.h"

#include "GUIManager.h"
#include "GUIStateManager.h"

#include "DotSceneLoader.h"

#include "AudioManager.h"
//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

MenuState::MenuState()
{
    m_bQuit         = false;
    m_FrameEvent    = Ogre::FrameEvent();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering MenuState...");

    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "MenuSceneMgr");

	//activar la GUI del main menu
	GUIManager::getSingletonPtr()->m_pGUIStateManager->
		changeGUIState( GUIManager::getSingletonPtr()->
		m_pGUIStateManager->findByName("GUIMainMenu") );

    createScene();

	AudioManager::getSingletonPtr()->playMainTrack("nightjob.ogg");
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::createScene()
{		

	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

	DotSceneLoader loader;
	//carga de la escena
	loader.parseDotScene("MainMenu.scene", "General", m_pSceneMgr);
	m_pCamera = m_pSceneMgr->getCamera("Camera#0");
	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
	OgreFramework::getSingletonPtr()->m_pViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));

	m_pSceneMgr->setSkyBox(true, "mainSkybox", 1000, true);
	
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	
	m_pCamNode = m_pSceneMgr->createSceneNode("camNode");
	m_pCamNode->attachObject(m_pCamera);

	
	//creacion de una luz direccional
	Ogre::Light* directionalLight = m_pSceneMgr->createLight("directionalLight");
    directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
    directionalLight->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
    directionalLight->setSpecularColour(Ogre::ColourValue(0, 0, 0.3));
	directionalLight->setDirection(Ogre::Vector3( 1, -1, 0 ));
	directionalLight->setPowerScale(10);
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::exit()
{
	AudioManager::getSingletonPtr()->unloadMainTrack();
	
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving MenuState...");

    
	m_pSceneMgr->destroyCamera(m_pCamera);

    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
		m_bQuit = true;
        return true;
    }

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::mouseMoved(const OIS::MouseEvent &evt)
{
	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);  
    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{

	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
    return true;
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::update(double timeSinceLastFrame)
{	   
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	
	m_pCamNode->rotate( Ogre::Vector3::UNIT_Y, Ogre::Degree
		(2*(timeSinceLastFrame/1000)));

    if(m_bQuit == true)
    {
        shutdown();
        return;
    }

	CEGUI::System::getSingleton().injectTimePulse( m_FrameEvent.timeSinceLastFrame);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

