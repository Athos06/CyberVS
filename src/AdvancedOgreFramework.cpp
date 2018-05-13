//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AdvancedOgreFramework.h"

#include "AppStateManager.h"

#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"
#include "GameOverState.h"

#include "GUIManager.h"
#include "AudioManager.h"
#include "GameManager.h"
//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> OgreFramework* Ogre::Singleton<OgreFramework>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::OgreFramework()
{
    m_pRoot				= 0;
    m_pRenderWnd		= 0;
    m_pViewport			= 0;
    m_pLog				= 0;
    m_pTimer			= 0;

    m_pInputMgr			= 0;
    m_pKeyboard			= 0;
    m_pMouse			= 0;
	
	m_pParticleSysMgr	= 0;
	
	m_pAppStateManager	= 0;

	m_pGUImgr			= 0;

	m_pAudioMgr			= 0;

	m_pGameConfig		= 0;

}

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::~OgreFramework()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");

    if(m_pInputMgr)		OIS::InputManager::destroyInputSystem(m_pInputMgr);
    if(m_pRoot)			delete m_pRoot;
	if(m_pAudioMgr)		delete m_pAudioMgr;
	if(m_pGameConfig)	delete m_pGameConfig;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
{
    Ogre::LogManager* logMgr = new Ogre::LogManager();

    m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
    m_pLog->setDebugOutputEnabled(true);

    m_pRoot = new Ogre::Root();

    if(!m_pRoot->showConfigDialog())
        return false;

	m_pRenderWnd = m_pRoot->initialise(true, wndTitle);

    m_pViewport = m_pRenderWnd->addViewport(0);
    m_pViewport->setBackgroundColour(ColourValue(0.5f, 0.5f, 0.5f, 1.0f));

    m_pViewport->setCamera(0);

    size_t hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);

    paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

    m_pInputMgr = OIS::InputManager::createInputSystem(paramList);

    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
    m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));

    m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
    m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();

    if(pKeyListener == 0)
        m_pKeyboard->setEventCallback(this);
    else
        m_pKeyboard->setEventCallback(pKeyListener);

    if(pMouseListener == 0)
        m_pMouse->setEventCallback(this);
    else
        m_pMouse->setEventCallback(pMouseListener);

    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    m_pTimer = new Ogre::Timer();
    m_pTimer->reset();
	
	//Ogre::LogManager* logMgr = new Ogre::LogManager();

    //m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);

	Ogre::ParticleSystemManager* particleMgr = Ogre::ParticleSystemManager::getSingletonPtr();//new Ogre::ParticleSystemManager();
	m_pParticleSysMgr = Ogre::ParticleSystemManager::getSingletonPtr(); //new Ogre::ParticleSystemManager();

	//manejador de la GUI
	m_pGUImgr = new GUIManager();
	m_pGUImgr->initGUI();
	
	m_pAudioMgr = new AudioManager();
	
	//iniciar el audio, si se produce algun error al inciiar sdl
	//se cierra la aplicacion
	if(!m_pAudioMgr->initAudio()){
		return false;
	}
	
	m_pGameConfig = new GameConfig();

    m_pRenderWnd->setActive(true);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::initAppStateMgr(){

	//Creacion e inicializacion de estados del juego
	m_pAppStateManager = new AppStateManager();

	MenuState::create(m_pAppStateManager, "MenuState");
	GameState::create(m_pAppStateManager, "GameState");
	PauseState::create(m_pAppStateManager, "PauseState");
	GameOverState::create(m_pAppStateManager, "GameOverState");

	m_pAppStateManager->start( m_pAppStateManager->findByName("MenuState") );

}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
    {
        m_pRenderWnd->writeContentsToTimestampedFile("Screenshot_", ".jpg");
        return true;
    }

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::updateOgre(double timeSinceLastFrame)
{
}

//|||||||||||||||||||||||||||||||||||||||||||||||