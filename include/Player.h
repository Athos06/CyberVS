//|||||||||||||||||||||||||||||||||||||||||||||||
#ifndef PLAYER_H
#define PLAYER_H
//|||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>
#include "GameObject.h"

#include "OgreBulletDynamicsRigidBody.h"	 // for OgreBullet
#include <btBulletCollisionCommon.h>

//|||||||||||||||||||||||||||||||||||||||||||||||

class Weapon;
class Proyectile;

//|||||||||||||||||||||||||||||||||||||||||||||||

class Player : public GameObject {
public:
	Player();
	Player(Ogre::SceneNode*, int);
	~Player();
	
	virtual void update(double);
	virtual void createPlayer(Ogre::SceneManager*);

	virtual void getHit();
	virtual void score();

	void rotateCannon(float);
	void beginShot();
	void releaseShot();

	void resetPosition();
	void resetPlayer();
	
	int getScore(){ return m_Score;};

	void setAvailableShot(bool available);

	float getPressedTime(){return m_ShootPressedTime;};

	bool mbFirstCollision;

	Player* m_pEnemy;
	Weapon* m_pWeapon;
	
	
	void setActiveProyectile(Proyectile* proy){ m_pActiveProyectile = proy;};

protected:
	int m_NumberPlayer; //guardamos el numero de jugador, 0 o 1 

	//variables para fuerza de disparo
	bool m_ShootActive;
	float m_ShootPressedTime;
	float m_ShotForce;

	//solo permitir un disparo a la vez
	bool m_ShotAvailable;

	const float m_MaxShootTime;
	const float m_MinShotForce;
	const float m_MaxShotForce;
	
	Ogre::Vector3 m_InitialPos;

	int m_Score;

	Ogre::ParticleSystem* m_pParticle;
		
	OgreBulletDynamics::RigidBody *m_pRigidBody;

	Proyectile* m_pActiveProyectile;
};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif
//|||||||||||||||||||||||||||||||||||||||||||||||