#include "PhysicsManager.h"

#include "AdvancedOgreFramework.h"
#include "GUIStateManager.h"
#include "AppStateManager.h"

#include "LevelManager.h"
#include "GameObject.h"
#include "ObstacleBar.h"
#include "Proyectile.h"
#include "Player.h"
//|||||||||||||||||||||||||||||||||||||||||||||||

template<> PhysicsManager* Ogre::Singleton<PhysicsManager>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

PhysicsManager& PhysicsManager::getSingleton(){
	assert (msSingleton);
	return (*msSingleton);
}

PhysicsManager* PhysicsManager::getSingletonPtr(){
	assert(msSingleton);
	return msSingleton;
}


PhysicsManager::PhysicsManager(){
	m_pWorld			= 0;	
 	m_pDebugDrawer		= 0;
	m_pSceneMgr			= 0;
}

PhysicsManager::PhysicsManager(Ogre::SceneManager* SceneMgr){
	m_pWorld			= 0;	
 	m_pDebugDrawer		= 0;
	m_pSceneMgr			= SceneMgr;
	
}

PhysicsManager::~PhysicsManager(){
	// OgreBullet physic delete - RigidBodies
    std::deque<OgreBulletDynamics::RigidBody *>::iterator itBody 
		= m_Bodies.begin();
	while (m_Bodies.end() != itBody)
	{   
		delete *itBody;
		++itBody;
	}   
	// OgreBullet physic delete - Shapes
	std::deque<OgreBulletCollisions::CollisionShape *>::iterator itShape = m_Shapes.begin();
	while (m_Shapes.end() != itShape)
	{   
		delete *itShape;
		++itShape;
	}
	m_Bodies.clear();
	m_Shapes.clear();
	delete m_pWorld->getDebugDrawer();
	m_pWorld->setDebugDrawer(0);
	delete m_pWorld;
}




void PhysicsManager::initPhysics()
{
	m_GravityVector = Ogre::Vector3(0,-9.81,0);
	//limites del mundo de bullet
	m_WorldBounds = Ogre::AxisAlignedBox( Ogre::Vector3(-10000, -10000, -10000), Ogre::Vector3(10000,  10000,  10000) );
	//mundo bullet
	m_pWorld = new OgreBulletDynamics::DynamicsWorld( m_pSceneMgr, m_WorldBounds, m_GravityVector);
	// add Debug info display tool
	m_pDebugDrawer = new OgreBulletCollisions::DebugDrawer();
	m_pDebugDrawer->setDrawWireframe(false);   // we want to see the Bullet containers
	m_pWorld->setDebugDrawer(m_pDebugDrawer);
	m_pWorld->setShowDebugShapes(false);      // enable it if you want to see the Bullet containers
	Ogre::SceneNode *node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
	node->attachObject( static_cast <Ogre::SimpleRenderable *> (m_pDebugDrawer) );
}


void PhysicsManager::CollisionsDetection(){

	int numManifolds = m_pWorld->getBulletDynamicsWorld()->
		getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++){
			//2
		btPersistentManifold* contactManifold =  m_pWorld->getBulletDynamicsWorld()->
 			getDispatcher()->getManifoldByIndexInternal(i);
	 
			//3
		btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());
		
		int numContacts = contactManifold->getNumContacts();
		//if (numContacts > 0)
		if(obA->getUserPointer() && obB->getUserPointer() ){
			GameObject* goA = (GameObject*)obA->getUserPointer();
			GameObject* goB = (GameObject*)obB->getUserPointer();
			
			//COLISION LIMITES DEL NIVEL
			if ( (goA->getTag() == "bound") && (goB->getTag() == "Proyectile") ) {
				Proyectile* proyectile = (Proyectile*)obB->getUserPointer();
				LevelManager::getSingletonPtr()->destroyProyectile(proyectile);
				break;			
			}

			//COLISION PROYECTIL CON PLAYER
			if ( (goA->getTag() == "Player") && (goB->getTag() == "Proyectile") ) {
				Player* player = (Player*)obA->getUserPointer();
				Proyectile* proyectile = (Proyectile*)obB->getUserPointer();
				//if(numContacts > 0 && !player->mbFirstCollision ){
					//player->mbFirstCollision = true;
					LevelManager::getSingletonPtr()->
						destroyProyectile(proyectile);
					player->m_pEnemy->score();
					player->getHit();
				//}
				//if(numContacts == 0 && player->mbFirstCollision){
				//	player->mbFirstCollision = false;
				//}	
				break;
			}
			//COLISION PROYECTIL CON OBSTACULO
			if ( (goA->getTag() == "Obstaculo") && (goB->getTag() == "Proyectile") ) {
				ObstacleBar* obstacle = (ObstacleBar*)obA->getUserPointer();
				Proyectile* proyectile = (Proyectile*)obB->getUserPointer();

				//if(numContacts > 0 && !obstacle->bFirstCollision ){
				//	obstacle->bFirstCollision = true;
					LevelManager::getSingletonPtr()->
						destroyProyectile(proyectile);
					obstacle->shrink(1);
				//}
				//if(numContacts == 0 && obstacle->bFirstCollision){
					//obstacle->bFirstCollision = false;
				//}			
				break;
				//m_pWorld->removeObject
			}
		}
	}
}
//|||||||||||||||||||||||||||||||||||||||||||||||
