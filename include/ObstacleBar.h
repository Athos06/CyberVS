//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OBSTACLE_BAR_H
#define OBSTACLE_BAR_H
//|||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>

#include "OgreBulletDynamicsRigidBody.h"				 // for OgreBullet
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h" // for static planes
#include "Shapes/OgreBulletCollisionsBoxShape.h"		 // for Boxes
#include <btBulletCollisionCommon.h>

#include "GameObject.h"
//|||||||||||||||||||||||||||||||||||||||||||||||

class ObstacleBar : public GameObject{
public:
	ObstacleBar();
	ObstacleBar(Ogre::SceneNode*, OgreBulletDynamics::RigidBody*,
		Ogre::String);
	~ObstacleBar();
	
	void createObstacle();
	void shrink(int);
	void reset();

	bool bFirstCollision; //se pone a true cuando se produce
	//la primera colision, false cuando termina la colision
private:
	OgreBulletDynamics::RigidBody *m_pRigidBody;
	int m_MaxPos;
	int m_Pos;
	
	Ogre::Vector3 m_InitPos;
};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif
//|||||||||||||||||||||||||||||||||||||||||||||||