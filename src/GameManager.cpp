#include "GameManager.h"

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"

#include "Euler.h"
#include "HUD.h"
#include "LevelManager.h"
#include "AIplayer.h"
#include "AudioManager.h"

template <> GameManager* Ogre::Singleton<GameManager>::msSingleton = 0;

GameManager& GameManager::getSingleton(){
	assert (msSingleton);
	return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GameManager* GameManager::getSingletonPtr(){
	assert (msSingleton);
	return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GameManager::GameManager(){
	m_pSceneMgr			= 0;

	m_Players[0]		= 0;
	m_Players[1]		= 0;
	
	m_pScore[0]			= 0;
	m_pScore[1]			= 0;
	
	m_pHUD				= 0;

	m_ScoreLimit		= 3;
	m_Winner			= -1;

	
	m_Ending			= false;
	m_NextRound			= false;
	m_GetReady			= false;

	m_ReadyTime			= 0;
	m_NextRoundTime		= 0;
	m_EndingTime		= 0;
	
	m_AIactivated[0] = false;
	m_AIactivated[1] = false;

	m_AISelDifficulty[0] = 0;
	m_AISelDifficulty[1] = 0;

		
}
//||||||||||||||||||||||||||||||||||||||||

GameManager::GameManager(Ogre::SceneManager* sceneMgr){
	m_pSceneMgr		= sceneMgr;
	
	m_Players[0]		= 0;
	m_Players[1]		= 0;

	m_pScore[0]			= 0;
	m_pScore[1]			= 0;
	
	m_pHUD				= 0;

	m_ScoreLimit		= 3;
	m_Winner			= -1;

	
	m_Ending			= false;
	m_NextRound			= false;
	m_GetReady			= false;

	m_ReadyTime			= 0;
	m_NextRoundTime		= 0;
	m_EndingTime		= 0;

	m_AIactivated[0] = false;
	m_AIactivated[1] = false;

	m_AISelDifficulty[0] = 0;
	m_AISelDifficulty[1] = 0;
}
//||||||||||||||||||||||||||||||||||||||||

GameManager::~GameManager(){
	if(m_Players[0])
		delete m_Players[0];
	if(m_Players[1])
		delete m_Players[1];
	if(m_pHUD)
		delete m_pHUD;
}
//||||||||||||||||||||||||||||||||||||||||

void GameManager::update(double timeSinceLastFrame){
	//update HUD and player
	m_pHUD->update();
	m_Players[0]->update(timeSinceLastFrame);
	m_Players[1]->update(timeSinceLastFrame);

	
	//timeSinceLastFrame va siempre en ms no olvidar
	//I dunno wut Im doin :D
	if(m_GetReady){
		m_ReadyTime += timeSinceLastFrame/1000;
		if(m_ReadyTime >= 3){
			m_pHUD->setMessage("go");
		}
		if(m_ReadyTime >= 4){
			m_ReadyTime = 0;
			m_GetReady = false;
			m_pHUD->showMessage(false);
		}
	}
	 
	if(m_NextRound){
		m_NextRoundTime += timeSinceLastFrame/1000;
		if(m_NextRoundTime >= 2){
			m_pHUD->setMessage("go");
		}
		if(m_NextRoundTime >= 3){
			m_NextRoundTime = 0;
			m_NextRound = false;
			m_pHUD->showMessage(false);
		}
	}

	//tiempo de espera para mostrar gameOver despues de que un jugador
	//haya alcanzado la puntuacion limite.
	//espera un tiempo para permitir ejecutar algun efecto antes de mostrar
	//pantalla de gameOver

	if(m_Ending){
		m_EndingTime += timeSinceLastFrame/1000;
		if(m_EndingTime >= 3){
			m_EndingTime = 0;
			m_Ending = false;
			gameOver();
		}
	} 

}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameManager::restartGame(){
	m_Players[0]->resetPlayer();
	m_Players[1]->resetPlayer();

	m_pScore[0] = 0;
	m_pScore[1] = 0;

	m_GetReady = true;

	m_pHUD->setMessage("ready");
	m_pHUD->showMessage(true);


	AudioManager::getSingletonPtr()->unloadMainTrack();
	AudioManager::getSingletonPtr()->playMainTrack("fruitful_12.ogg");
	

	LevelManager::getSingletonPtr()->resetLevel();

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GameManager::initGame(){

	m_pHUD = new HUD();
	
	m_ScoreLimit = GameConfig::getSingletonPtr()->limitScore;

	m_AIactivated[0] = GameConfig::getSingletonPtr()->AIon[0];
	m_AIactivated[1] = GameConfig::getSingletonPtr()->AIon[1];

	m_AISelDifficulty[0] = GameConfig::getSingletonPtr()->AIdifficulty[0];
	m_AISelDifficulty[1] = GameConfig::getSingletonPtr()->AIdifficulty[1];

	createPlayer();
	
	m_Players[0]->resetPlayer();
	m_Players[1]->resetPlayer();


	m_GetReady = true;

	m_pHUD->setMessage("ready");
	m_pHUD->showMessage(true);

}
void GameManager::createPlayer(){
	
	Ogre::SceneNode* node = m_pSceneMgr->getSceneNode("Player#0");
	//si hemos marcado que este controlado por la IA se crea un jugador
	//con IA, sino se crea uno controlado por el jugador humano
	if(m_AIactivated[0]){
		m_Players[0] = new AIplayer(node,0, m_AISelDifficulty[0]);
		m_Players[0]->createPlayer(m_pSceneMgr);
	}
	else{
		m_Players[0] = new Player(node,0);
		m_Players[0]->createPlayer(m_pSceneMgr);
	}
	Ogre::SceneNode* node2 = m_pSceneMgr->getSceneNode("Player#1");

	if(m_AIactivated[1]){
		m_Players[1] = new AIplayer(node2,1, m_AISelDifficulty[1]);
		m_Players[1]->createPlayer(m_pSceneMgr);
	}
	else{
		m_Players[1] = new Player(node2,1);
		m_Players[1]->createPlayer(m_pSceneMgr);
	}
	
	m_Players[0]->m_pEnemy = m_Players[1];
	m_Players[1]->m_pEnemy = m_Players[0];

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GameManager::nextRound(){
	m_NextRound = true;
	m_pHUD->setMessage("nextRound");
	m_pHUD->showMessage(true);
	LevelManager::getSingletonPtr()->resetLevel();
	m_Players[0]->resetPosition();
	m_Players[1]->resetPosition();
}
//|||||||||||||||||||||||||||||||||||||||||||||||


void GameManager::updateScore(){
	m_pScore[0] = m_Players[0]->getScore();
	m_pScore[1] = m_Players[1]->getScore();

	if(m_pScore[0] >= m_ScoreLimit)
	{
		//establecer al player 1 como ganador
		m_Winner = 0;
		m_Ending = true;
		m_pHUD->setMessage("p1wins");
		m_pHUD->showMessage(true);
		return;
	}
	if(m_pScore[1] >= m_ScoreLimit)
	{
		//establecer al player 2 como ganador
		m_Winner = 1;
		m_Ending = true;
		m_pHUD->setMessage("p2wins");
		m_pHUD->showMessage(true);
		return;
	}
	
	nextRound();
	
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameManager::gameOver(){
	m_pHUD->showMessage(false);

	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		pushAppState( OgreFramework::getSingletonPtr()->m_pAppStateManager->
		findByName("GameOverState") );
		
}
//|||||||||||||||||||||||||||||||||||||||||||||||

int* GameManager::getScore(){
	return m_pScore;
}
//|||||||||||||||||||||||||||||||||||||||||||||||



template <> GameConfig* Ogre::Singleton<GameConfig>::msSingleton = 0;

GameConfig& GameConfig::getSingleton(){
	assert (msSingleton);
	return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GameConfig* GameConfig::getSingletonPtr(){
	assert (msSingleton);
	return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||
GameConfig::GameConfig(){
	limitScore = 3;
	volume = 100;
	AIon[0] = false;
	AIon[1] = false;
	AIdifficulty[0]	= 0;
	AIdifficulty[1]	= 0;
};
//|||||||||||||||||||||||||||||||||||||||||||||||

GameConfig::~GameConfig(){};
//|||||||||||||||||||||||||||||||||||||||||||||||