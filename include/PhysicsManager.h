//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef	PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H
//|||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>

#include "OgreBulletDynamicsRigidBody.h"				 // for OgreBullet
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h" // for static planes
#include "Shapes/OgreBulletCollisionsBoxShape.h"		 // for Boxes

#include <btBulletCollisionCommon.h>
//|||||||||||||||||||||||||||||||||||||||||||||||

class PhysicsManager :  public Ogre::Singleton<PhysicsManager>{
public:
	static PhysicsManager& getSingleton();
	static PhysicsManager* getSingletonPtr();

	PhysicsManager();
	PhysicsManager(Ogre::SceneManager*);
	~PhysicsManager();

	void initPhysics();

	void CollisionsDetection();

	OgreBulletDynamics::DynamicsWorld *m_pWorld;	// OgreBullet World
 	OgreBulletCollisions::DebugDrawer *m_pDebugDrawer;

	std::deque<OgreBulletDynamics::RigidBody *>         m_Bodies;
 	std::deque<OgreBulletCollisions::CollisionShape *>  m_Shapes;
	
	int m_NumEntitiesInstanced; //para crear nombres unicos sin conflicto


private:
	Ogre::SceneManager* m_pSceneMgr;
	Ogre::AxisAlignedBox m_WorldBounds; 
	Ogre::Vector3 m_GravityVector;
	

};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif
//|||||||||||||||||||||||||||||||||||||||||||||||