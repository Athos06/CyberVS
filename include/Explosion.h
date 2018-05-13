#ifndef __EXPLOSION__H
#define __EXPLOSION__H

#include <Ogre.h>
#include "GameObject.h"

class Explosion : public GameObject{
public:
	Explosion();
	Explosion(float, Ogre::String, Ogre::Vector3);
	~Explosion();

	void destroy();
	void update(double);
	

private:
	float m_AliveTime;
	float m_LifeTime;
	Ogre::ParticleSystem* m_pParticle;
	//Ogre::SceneManager* m_pSceneMgr;

	bool once;

};

#endif 
