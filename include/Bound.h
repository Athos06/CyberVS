#ifndef BOUND_H
#define BOUND_H

#include <Ogre.h>
#include "GameObject.h"
#include "OgreBulletDynamicsRigidBody.h"	

class Bound : public GameObject{
public:
	Bound();
	Bound(Ogre::SceneNode*, OgreBulletDynamics::RigidBody*);
	~Bound();

	OgreBulletDynamics::RigidBody *m_pRigidBody;
};
 
#endif