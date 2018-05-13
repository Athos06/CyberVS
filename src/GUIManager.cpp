
#include <CEGUI.h>

#include "AdvancedOgreFramework.h"

#include "GUIManager.h"
#include "GUIStateManager.h"
#include "GUIMainMenu.h"
#include "GUICredits.h"
#include "GUIGame.h"
#include "GUIPauseMenu.h"
#include "GUIGameOver.h"
#include "GUIOptions.h"
#include "GUIAudio.h"
#include "GUIControls.h"
#include "GUISelectPlayers.h"
#include "GUISelectRounds.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUIManager* Ogre::Singleton<GUIManager>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||


GUIManager& GUIManager::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

 
GUIManager* GUIManager::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIManager::GUIManager(){
	m_pGUIRenderer		= 0;
    m_pGUISystem		= 0;

	m_pGUIStateManager = 0;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::initGUI(){
	m_pGUIRenderer = &CEGUI::OgreRenderer::bootstrapSystem( *(OgreFramework::getSingletonPtr()->m_pRenderWnd) );
    m_pGUISystem = CEGUI::System::getSingletonPtr();
	
	//creacion del manejador de estados de la GUI (debe crearse antes
	//de crear la GUI puesto que se requiere de los estados creados para
	//suscribir los metodos de los botones de la GUI)
	m_pGUIStateManager = new GUIStateManager();
	
	GUIMainMenu::create(m_pGUIStateManager, "GUIMainMenu");
	GUICredits::create(m_pGUIStateManager, "GUICredits");
	GUIGame::create(m_pGUIStateManager, "GUIGame");
	GUIPauseMenu::create(m_pGUIStateManager, "GUIPauseMenu");
	GUIGameOver::create(m_pGUIStateManager, "GUIGameOver");
	GUIOptions::create(m_pGUIStateManager, "GUIOptions");
	GUIAudio::create(m_pGUIStateManager, "GUIAudio");
	GUIControls::create(m_pGUIStateManager, "GUIControls");
	GUISelectPlayers::create(m_pGUIStateManager, "GUISelectPlayers");
	GUISelectRounds::create(m_pGUIStateManager, "GUISelectRounds");
	
	createGUI();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::createGUI(){
	CreateMainMenuGUI();
	CreateInGameGUI();
	CreateCreditsGUI();
	CreatePauseMenuGUI();
	CreateGameOverGUI();
	CreateOptionsGUI();
	CreateAudioGUI();
	CreateControlsGUI();
	CreateSelectPlayersGUI();
	CreateSelectRoundsGUI();

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::CreateMainMenuGUI(){

	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	m_pGUISystem->setDefaultMouseCursor("TaharezLook","MouseArrow"); 
	
	//cargar la GUI sheet del main menu
	m_pGUIsheetMainMenu = CEGUI::WindowManager::getSingleton().loadWindowLayout("MainMenu.layout");


	//asociar los botones del main menu con las funciones correspondientes a las que llaman
	CEGUI::Window* exitButton = CEGUI::WindowManager::getSingleton().getWindow("MainMenuRoot/ExitButton");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIMainMenu::onExitButton, 
		GUIMainMenu::getSingletonPtr() ));

	CEGUI::Window* creditsButton = CEGUI::WindowManager::getSingleton().getWindow("MainMenuRoot/CreditsButton");
	creditsButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIMainMenu::onCreditsButton, 
		GUIMainMenu::getSingletonPtr() ));
	
	CEGUI::Window* newGameButton = CEGUI::WindowManager::getSingleton().getWindow("MainMenuRoot/PlayButton");
	newGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber( &GUIMainMenu::onPlayButton, 
		GUIMainMenu::getSingletonPtr() ) );  
	
	CEGUI::Window* optionsButton = CEGUI::WindowManager::getSingleton().getWindow("MainMenuRoot/OptionsButton");
	optionsButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIMainMenu::onOptionsButton, 
		GUIMainMenu::getSingletonPtr() ));
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::CreateInGameGUI(){
	m_pGUIsheetInGame = CEGUI::WindowManager::getSingleton().loadWindowLayout("InGame.layout");
}

//inicializa la GUI sheet de los creditos
void GUIManager::CreateCreditsGUI(){
	
	//cargar la GUI sheet del main menu
	m_pGUIsheetCredits = CEGUI::WindowManager::getSingleton().loadWindowLayout("Credits.layout");

	
	//asociar los botones del main menu con las funciones correspondientes a las que llaman
	CEGUI::Window* backButton = CEGUI::WindowManager::getSingleton().getWindow
		("CreditsRoot/Background/ExitButton");
	backButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUICredits::onBackButton, 
		GUICredits::getSingletonPtr() ));
		
}

//|||||||||||||||||||||||||||||||||||||||||||||||

//inicializa la GUI sheet del menu de pausa
void GUIManager::CreatePauseMenuGUI(){
	
	//cargar la GUI sheet del menu de pausa
	m_pGUIsheetPauseMenu = CEGUI::WindowManager::getSingleton().loadWindowLayout("PauseMenu.layout");

	//asociar los botones del main menu con las funciones correspondientes a las que llaman
	CEGUI::Window* backButton = CEGUI::WindowManager::getSingleton().getWindow
		("PauseMenuRoot/ResumeButton");
	backButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIPauseMenu::onResumeButton, 
		GUIPauseMenu::getSingletonPtr() ));

	CEGUI::Window* optionsButton = CEGUI::WindowManager::getSingleton().getWindow
		("PauseMenuRoot/OptionsButton");
	optionsButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIPauseMenu::onOptionsButton, 
		GUIPauseMenu::getSingletonPtr() ));

	CEGUI::Window* MainMenuButton = CEGUI::WindowManager::getSingleton().getWindow
		("PauseMenuRoot/BackToMainButton");
	MainMenuButton->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GUIPauseMenu::onMainMenuButton,
		GUIPauseMenu::getSingletonPtr() ));

	CEGUI::Window* exitButton = CEGUI::WindowManager::getSingleton().getWindow
		("PauseMenuRoot/ExitButton");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GUIPauseMenu::onExitButton,
		GUIPauseMenu::getSingletonPtr() ));
}


//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::CreateGameOverGUI(){
	
	//cargar la GUI sheet del main menu
	m_pGUIGameOver = CEGUI::WindowManager::getSingleton().loadWindowLayout("GameOver.layout");


	//asociar los botones del main menu con las funciones correspondientes a las que llaman
	CEGUI::Window* BackToMenuButton = CEGUI::WindowManager::getSingleton().getWindow
		("GameOverRoot/bg/BackToMenuButton");
	BackToMenuButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIGameOver::onBackButton, 
		GUIGameOver::getSingletonPtr() ));

	CEGUI::Window* playAgainButton = CEGUI::WindowManager::getSingleton().getWindow
		("GameOverRoot/bg/PlayAgainButton");
	playAgainButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIGameOver::onPlayAgainButton, 
		GUIGameOver::getSingletonPtr() ));

}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::CreateOptionsGUI(){
	//cargar la GUI sheet del main menu
	m_pGUIsheetOptions = CEGUI::WindowManager::getSingleton().loadWindowLayout("OptionsMenu.layout");

	CEGUI::Window* AudioButton = CEGUI::WindowManager::getSingleton().getWindow
		("OptionsRoot/AudioButton");
	AudioButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIOptions::onAudioButton, 
		GUIOptions::getSingletonPtr() ));



	//asociar los botones del main menu con las funciones correspondientes a las que llaman
	CEGUI::Window* ControlsButton = CEGUI::WindowManager::getSingleton().getWindow
		("OptionsRoot/ControlsButton");
	ControlsButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIOptions::onControlsButton, 
		GUIOptions::getSingletonPtr() ));

	CEGUI::Window* BackButton = CEGUI::WindowManager::getSingleton().getWindow
		("OptionsRoot/BackButton");
	BackButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIOptions::onBackButton, 
		GUIOptions::getSingletonPtr() ));
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::CreateAudioGUI(){
	m_pGUIsheetAudio = CEGUI::WindowManager::getSingleton().loadWindowLayout("Audio.layout");

	CEGUI::Window* volume = CEGUI::WindowManager::getSingleton().getWindow
		("AudioRoot/VolumeControl");
	volume->subscribeEvent(CEGUI::Spinner::EventValueChanged, 
		CEGUI::Event::Subscriber(&GUIAudio::volumeChange, 
		GUIAudio::getSingletonPtr() ));
	
	CEGUI::Window* BackButton = CEGUI::WindowManager::getSingleton().getWindow
		("AudioRoot/BackButton");
	BackButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIAudio::onBackButton, 
		GUIAudio::getSingletonPtr() ));
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::CreateControlsGUI(){
	//cargar la GUI sheet del main menu
	m_pGUIsheetControls = CEGUI::WindowManager::getSingleton().loadWindowLayout("Controls.layout");


	//asociar los botones del main menu con las funciones correspondi	entes a las que llaman
	CEGUI::Window* BackButton = CEGUI::WindowManager::getSingleton().getWindow
		("ControlsRoot/StaticImage/BackButton");
	BackButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIControls::onBackButton, 
		GUIControls::getSingletonPtr() ));
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::CreateSelectPlayersGUI(){
	//cargar la GUI sheet del main menu
	m_pGUIsheetSelectPlayers = CEGUI::WindowManager::getSingleton().loadWindowLayout("SelectMode.layout");
	//asociar los botones del main menu con las funciones correspondientes a las que llaman
	CEGUI::Window* BackButton = CEGUI::WindowManager::getSingleton().getWindow
		("SelectModeRoot/BackButton");
	BackButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUISelectPlayers::onBackButton, 
		GUISelectPlayers::getSingletonPtr() ));
	
	CEGUI::Window* PVPButton = CEGUI::WindowManager::getSingleton().getWindow
		("SelectModeRoot/PVPButton");
	PVPButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUISelectPlayers::onPVPButton, 
		GUISelectPlayers::getSingletonPtr() ));
	
	CEGUI::Window* PVsAIButton = CEGUI::WindowManager::getSingleton().getWindow
		("SelectModeRoot/PlayerVSAIButton");
	PVsAIButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUISelectPlayers::onPvsAIButton, 
		GUISelectPlayers::getSingletonPtr() ));
		
	CEGUI::Window* AIVsAIButton = CEGUI::WindowManager::getSingleton().getWindow
		("SelectModeRoot/AIvsAIButton");
	AIVsAIButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUISelectPlayers::onAIvsAIButton, 
		GUISelectPlayers::getSingletonPtr() ));
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::CreateSelectRoundsGUI(){
	m_pGUIsheetSelectRounds = CEGUI::WindowManager::getSingleton().loadWindowLayout("SelectRounds.layout");

	CEGUI::Window* StartButton = CEGUI::WindowManager::getSingleton().getWindow
		("SelectRoundsRoot/StaticImage/StartButton");
	StartButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUISelectRounds::onPlayButton, 
		GUISelectRounds::getSingletonPtr() ));

	CEGUI::Window* BackButton = CEGUI::WindowManager::getSingleton().getWindow
		("SelectRoundsRoot/StaticImage/BackButton");
	BackButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUISelectRounds::onBackButton, 
		GUISelectRounds::getSingletonPtr() ));
}
//|||||||||||||||||||||||||||||||||||||||||||||||