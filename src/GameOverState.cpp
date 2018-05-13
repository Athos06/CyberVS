//|||||||||||||||||||||||||||||||||||||||||||||||

#include "GameOverState.h"

#include "GUIManager.h"
#include "GUIStateManager.h"
#include "GameManager.h"
#include "HUD.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

GameOverState::GameOverState()
{
    m_bQuit             = false;
    m_bQuestionActive   = false;
    m_FrameEvent        = Ogre::FrameEvent();
	m_pBackground		= 0;
	m_pNodeBg			= 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameOverState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering GameOverState...");
	
	//ocultar el HUD
	GameManager::getSingletonPtr()->m_pHUD->hide();
	//cambiar la GUI
	GUIManager::getSingletonPtr()->m_pGUIStateManager->
		pushGUIState(GUIManager::getSingletonPtr()->m_pGUIStateManager->findByName("GUIGameOver") );

    m_bQuit = false;

    createScene();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameOverState::createScene()
{
	Ogre::SceneManager* sceneMgr = GameManager::getSingletonPtr()->m_pSceneMgr;

	// Create background m_pBackgroundangle covering the whole screen
	m_pBackground = new Ogre::Rectangle2D(true);
	m_pBackground->setCorners(-1.0, 1.0, 1.0, -1.0);
	m_pBackground->setMaterial("Background");
	// Render the background before everything else
	m_pBackground->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAX);
	// Use infinite AAB to always stay visible
	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	m_pBackground->setBoundingBox(aabInf);	 
	// Attach background to the scene
	m_pNodeBg = sceneMgr->getRootSceneNode()->createChildSceneNode("Background");
	m_pNodeBg->attachObject(m_pBackground);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameOverState::exit()
{
	//volver a mostrar el overlay al salir
	GameManager::getSingletonPtr()->m_pHUD->show();
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving GameOverState...");
	
	if(m_pBackground)	delete m_pBackground;
	if(m_pNodeBg){
		GameManager::getSingletonPtr()->m_pSceneMgr->destroySceneNode(m_pNodeBg);
	}		

	//desapilar la GUI del menu de pausa, volviendo a la gui del juego	
	GUIManager::getSingletonPtr()->m_pGUIStateManager->popGUIState();
	
}


//|||||||||||||||||||||||||||||||||||||||||||||||
bool GameOverState::onExitButtonGame(const CEGUI::EventArgs &args)
{
	m_bQuit = true;
	return true;
}

bool GameOverState::keyPressed(const OIS::KeyEvent &keyEventRef)
{

	CEGUI::System::getSingleton().injectKeyDown(keyEventRef.key);

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameOverState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	CEGUI::System::getSingleton().injectKeyUp(keyEventRef.key);

    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameOverState::mouseMoved(const OIS::MouseEvent &evt)
{
	
	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);  
	
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameOverState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{	
	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
	
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameOverState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
  
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameOverState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
  
    if(m_bQuit == true)
    {
        popAppState();
        return;
    }

	CEGUI::System::getSingleton().injectTimePulse( m_FrameEvent.timeSinceLastFrame);
}

void GameOverState::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit)
{
	if(yesHit == true)
        shutdown();


    m_bQuestionActive = false;
}

//|||||||||||||||||||||||||||||||||||||||||||||||