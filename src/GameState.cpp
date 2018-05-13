//|||||||||||||||||||||||||||||||||||||||||||||||

#include "GameState.h"

#include "GUIManager.h"
#include "GUIStateManager.h"
#include "AppStateManager.h"
#include "LevelManager.h"
#include "Proyectile.h"
#include <btBulletCollisionCommon.h>
#include "Player.h"
#include "Euler.h"
#include "GameManager.h"
#include "ExplosionsManager.h"
#include "AudioManager.h"
#include "Constants.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

GameState::GameState()
{
    m_bQuit             = false;
	
	m_pPhysicsMgr		= 0;
	m_pLevelMgr			= 0;
	m_pGameMgr			= 0;
	m_pExplosionsMgr	= 0;

}
//|||||||||||||||||||||||||||||||||||||||||||||||

GameState::~GameState(){

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering GameState...");

	m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(Ogre::ST_GENERIC, "GameSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
	

	GUIManager::getSingletonPtr()->m_pGUIStateManager->
		changeGUIState(GUIManager::getSingletonPtr()->m_pGUIStateManager->findByName("GUIGame") );

    createScene();

	AudioManager::getSingletonPtr()->playMainTrack("fruitful_12.ogg");

	srand(time(0));
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::createScene()
{
	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);

	DotSceneLoader loader;
	loader.parseDotScene("level1.scene", "General", m_pSceneMgr);
	m_pCamera = m_pSceneMgr->getCamera("Camera#0");
	m_pCamera->setFOVy(Ogre::Radian(0.25));

	m_pCamera->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));

	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
	m_pCamera->setFarClipDistance (500);
	
	m_pCamera->setFOVy(Ogre::Degree(45)); 
	m_pCamera->setPosition(0,5,20);
	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	m_pSceneMgr->setSkyBox(true, "gameSkybox", 10000, true);

	Ogre::Light* directionalLight = m_pSceneMgr->createLight("directionalLight");
    directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
    directionalLight->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
    directionalLight->setSpecularColour(Ogre::ColourValue(.25, .25, 0));
	directionalLight->setDirection(Ogre::Vector3( 0, -1.2, -1 ));
	directionalLight->setPowerScale(10);

	//iniciar la simulacion de fisicas
	m_pPhysicsMgr = new PhysicsManager(m_pSceneMgr);
	m_pPhysicsMgr->initPhysics();
	//manejador de explosiones
	m_pExplosionsMgr = new ExplosionsManager(m_pSceneMgr);
	//iniciar manejador de nivel
	m_pLevelMgr = new LevelManager(m_pSceneMgr, m_pPhysicsMgr);
	m_pLevelMgr->createLevel();
	//iniciar manejador del juego
	m_pGameMgr = new GameManager(m_pSceneMgr);
	m_pGameMgr->initGame();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::pause()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing GameState...");
    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::resume()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming GameState...");

    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

    m_bQuit = false;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::exit()
{
	AudioManager::getSingletonPtr()->unloadMainTrack();

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving GameState...");
	
	if(m_pPhysicsMgr)
		delete m_pPhysicsMgr;
	if(m_pLevelMgr)
		delete m_pLevelMgr;
	if(m_pGameMgr)
		delete m_pGameMgr;
	if(m_pExplosionsMgr)
		delete m_pExplosionsMgr;

	//eliminar el scene manager lo ultimo porque si no elimnia todos los nodos
	//y da error al intentar eliminar los nodos en los destructores de los otros objetos
	m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{

	CEGUI::System::getSingleton().injectKeyDown(keyEventRef.key);

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        pushAppState(findByName("PauseState"));
        return true;
    }

	if(!GameManager::getSingletonPtr()->m_AIactivated[0]){
		if(keyEventRef.key == OIS::KC_SPACE){
			GameManager::getSingletonPtr()->m_Players[0]->beginShot();  
		}
	}
	if(!GameManager::getSingletonPtr()->m_AIactivated[1]){	
		if(keyEventRef.key == OIS::KC_NUMPAD0){
				GameManager::getSingletonPtr()->m_Players[1]->beginShot();  
		}
	}
    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	CEGUI::System::getSingleton().injectKeyUp(keyEventRef.key);
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
	if(!GameManager::getSingletonPtr()->m_AIactivated[0]){	
		if(keyEventRef.key == OIS::KC_SPACE){
			GameManager::getSingletonPtr()->m_Players[0]->releaseShot();  
		}
	}
	if(!GameManager::getSingletonPtr()->m_AIactivated[1]){	
		if(keyEventRef.key == OIS::KC_NUMPAD0){
			GameManager::getSingletonPtr()->m_Players[1]->releaseShot();  
		}
	}
    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::getInput()
{
	float deltaTime = m_FrameEvent.timeSinceLastFrame/1000;
	
	
	if(!( m_pGameMgr->isGettingReady() 
		|| m_pGameMgr->isRoundTransition()
		|| m_pGameMgr->isEnding() ) ){
		if(!GameManager::getSingletonPtr()->m_AIactivated[0]){		
			if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W)){
				GameManager::getSingletonPtr()->m_Players[0]->rotateCannon(CANNON_ROT_SPEED*deltaTime);
			}
			if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S)){
				GameManager::getSingletonPtr()->m_Players[0]->rotateCannon(-CANNON_ROT_SPEED*deltaTime); 
			}
		}
		if(!GameManager::getSingletonPtr()->m_AIactivated[1]){	
			if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_UP)){
				GameManager::getSingletonPtr()->m_Players[1]->rotateCannon(CANNON_ROT_SPEED*deltaTime);
			}
			if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_DOWN)){
				GameManager::getSingletonPtr()->m_Players[1]->rotateCannon(-CANNON_ROT_SPEED*deltaTime); 
			}
		}
	}
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mouseMoved(const OIS::MouseEvent &evt)
{

	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);  

    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{

    if(id == OIS::MB_Left)
    {
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
    }
    else if(id == OIS::MB_Right)
    {
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
    }

    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(id == OIS::MB_Left)
    {
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
    }
    else if(id == OIS::MB_Right)
    {
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
    }

    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||


void GameState::update(double timeSinceLastFrame)
{
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	
    if(m_bQuit == true)
    {
        popAppState();
        return;
    }

	CEGUI::System::getSingleton().injectTimePulse( m_FrameEvent.timeSinceLastFrame);

    getInput();

	m_pGameMgr->update(timeSinceLastFrame);

	//actualizar bullet
	m_pPhysicsMgr->m_pWorld->stepSimulation(m_FrameEvent.timeSinceLastFrame*0.001, 2);

		
	//comprobar colisiones
	m_pPhysicsMgr->CollisionsDetection();

	m_pExplosionsMgr->update(timeSinceLastFrame);

}
//|||||||||||||||||||||||||||||||||||||||||||||||
