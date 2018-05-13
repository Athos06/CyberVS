#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include <Ogre.h>

#include <Ogre.h>

#include "OgreBulletDynamicsRigidBody.h"	 // for OgreBullet
#include <btBulletCollisionCommon.h>

class GameObject;
class Player;
class AIinput;

class AIplayer : public Player{
public:
	AIplayer();
	AIplayer(Ogre::SceneNode*, int, int);
	~AIplayer();
	
	void update(double);
	void createAIplayer(Ogre::SceneManager*);
	void createPlayer(Ogre::SceneManager*);
	void getAIinput();

	void getHit();
	void score();

	float getCurrentShotPower(){
		return m_MinShotForce + (m_ShootPressedTime * m_MaxShotForce)
			/(m_MaxShootTime);
	};

	Ogre::Quaternion getCannonRotation();

private:
	int m_difficulty;

	AIinput* m_pAIinput;

	
};

#endif 