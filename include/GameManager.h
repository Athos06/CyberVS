#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <Ogre.h>

#include "Player.h"

class HUD;

class GameManager : public Ogre::Singleton<GameManager>{
public:
	static GameManager& getSingleton();
	static GameManager* getSingletonPtr();

	GameManager();
	GameManager(Ogre::SceneManager*);
	~GameManager();

	void update(double);

	void initGame();
	void createPlayer();
		
	void updateScore();
	void gameOver();
	//funcion para obetener el array del score
	int* getScore();
	int getWinner(){return m_Winner;};
	int getScoreLimit(){return m_ScoreLimit;};
	void nextRound();

	void restartGame();

	void setEnding(bool value){m_Ending = value;};
	void RoundTransition(bool value){m_NextRound = value;};
	void GettingReady(bool value){m_NextRound = value;};
	bool isEnding(){return m_Ending;};
	bool isRoundTransition(){return m_NextRound;};
	bool isGettingReady(){return m_GetReady;};
	
	Ogre::SceneManager* m_pSceneMgr;
	
	Player* m_Players[2];

	HUD* m_pHUD;
	
	bool m_AIactivated[2];
	int m_AISelDifficulty[2];

private:
	int m_pScore[2];
	int m_ScoreLimit;
	int m_Winner;

	bool m_Ending;
	bool m_NextRound;
	bool m_GetReady;

	float m_EndingTime;
	float m_NextRoundTime;
	float m_ReadyTime;
};

class GameConfig : public Ogre::Singleton<GameConfig>{
public:
	static GameConfig& getSingleton();
	static GameConfig* getSingletonPtr();

	GameConfig();
	~GameConfig();

	int limitScore;
	int volume;
	bool AIon[2];

	int AIdifficulty[2];
};

#endif