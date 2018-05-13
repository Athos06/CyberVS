//|||||||||||||||||||||||||||||||||||||||||||||||
#ifndef PROYECTILE_H
#define PROYECTILE_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>

#include "OgreBulletDynamicsRigidBody.h"				 // for OgreBullet
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h" // for static planes
#include "Shapes/OgreBulletCollisionsBoxShape.h"		 // for Boxes
#include <btBulletCollisionCommon.h>

#include "GameObject.h"
#include "Player.h"
//|||||||||||||||||||||||||||||||||||||||||||||||

class Proyectile : public GameObject{
public:
	Proyectile();
	Proyectile(Ogre::SceneNode*, Ogre::String,
		OgreBulletDynamics::RigidBody*, Player* );
	~Proyectile();

	void createProyectile();	
	void destroy(Ogre::SceneManager*);
private:
	OgreBulletDynamics::RigidBody* m_pRigidBody;
	Player* m_pOwner;
};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif
//|||||||||||||||||||||||||||||||||||||||||||||||