#include "LevelManager.h"
#include "PhysicsManager.h"

#include "ObstacleBar.h"

#include <stdlib.h>
#include <time.h>

#include "Bound.h"

#include "ExplosionsManager.h"

#include <Ogre.h>
//|||||||||||||||||||||||||||||||||||||||||||||||

template<> LevelManager* Ogre::Singleton<LevelManager>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

LevelManager& LevelManager::getSingleton(){
	assert (msSingleton);
	return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

LevelManager* LevelManager::getSingletonPtr(){
	assert(msSingleton);
	return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

LevelManager::LevelManager(){
	m_pSceneMgr		= 0;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

LevelManager::LevelManager(Ogre::SceneManager* sceneMgr, PhysicsManager* PhysicsMgr){
	m_pSceneMgr = sceneMgr;
	m_pPhysicsMgr = PhysicsMgr;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

LevelManager::~LevelManager(){
	std::deque<ObstacleBar*>::iterator it 
		= m_Obstacles.begin();
	while (m_Obstacles.end() != it)
	{   
		delete *it;
		++it;
	}   
	m_Obstacles.clear();

	std::deque<Bound*>::iterator it2 
		= m_Bounds.begin();
	while (m_Bounds.end() != it2)
	{   
		delete *it2;
		++it2;
	}   
	m_Bounds.clear();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void LevelManager::pushObstacle(ObstacleBar* obstacle){
	m_Obstacles.push_back(obstacle);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

//crea los limites del nivel, con colliders para que se destruya
//el proyectil al salir
void LevelManager::createBounds(){
		
	
	//plano del suelo
	Ogre::Entity *ent;
	Ogre::Plane p;
	p.normal = Ogre::Vector3(0,1,0); 
	p.d = 0;
	Ogre::MeshManager::getSingleton().createPlane("FloorPlane", 
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		p, 100, 100, 4, 4, true, 1, 7, 7, Ogre::Vector3::UNIT_Z);
    // Create an entity (the floor)
    ent = m_pSceneMgr->createEntity("floorBound", "FloorPlane");
    ent->setMaterialName("groundGrid");
	Ogre::SceneNode* nodoBound1 = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("floorBound");
	nodoBound1->attachObject(ent);
		

	/////////LIMITE DE LA IZQUIERDA///////////////////
	//////////////////////////////////////////////////
	Ogre::Vector3 size = Ogre::Vector3::ZERO;   // size of the box
	
	Ogre::Entity* leftBoundEnt = m_pSceneMgr->
			createEntity("leftBoundEnt",
			Ogre::SceneManager::PT_CUBE);	
	Ogre::SceneNode* leftBoundNode = m_pSceneMgr->getRootSceneNode()->
			createChildSceneNode("leftBoundNode");
	Ogre::AxisAlignedBox boundingB = leftBoundEnt->getBoundingBox();
	size = boundingB.getSize(); size /= 2.0f; // only the half needed
	size *= 0.96f;
	leftBoundNode->attachObject(leftBoundEnt);
	leftBoundNode->setPosition(-12,0,0);
	leftBoundNode->setScale(0.02,50,10);
	Ogre::Vector3 barraPos = leftBoundNode->getPosition();
	size = Ogre::Vector3(size.x * 0.02, size.y*50 , size.z*10); 
	//AHORA LA PARTE DE FISICAS
	OgreBulletCollisions::BoxCollisionShape *sceneBoxShape = 
		new OgreBulletCollisions::BoxCollisionShape(size);
	// and the Bullet rigid body
	OgreBulletDynamics::RigidBody *leftBody = new OgreBulletDynamics::RigidBody(
		"leftBoundBody" + Ogre::StringConverter::toString
			(m_pPhysicsMgr->m_NumEntitiesInstanced),
				m_pPhysicsMgr->m_pWorld);
	leftBody->setShape(leftBoundNode, sceneBoxShape,
						   0,         // dynamic body restitution
						   0,         // dynamic body friction
						   0,          // dynamic bodymass
						   barraPos,      // starting position of the box
						   Ogre::Quaternion(0,0,0,1));// orientation of the box        
	// push the created objects to the deques
	m_pPhysicsMgr->m_Shapes.push_back(sceneBoxShape);
	m_pPhysicsMgr->m_NumEntitiesInstanced++;

	Bound* bound2 = new Bound(leftBoundNode,leftBody);
	m_Bounds.push_back(bound2);
	leftBody->getBulletRigidBody()->setUserPointer(bound2);

	//no necesito que se vea, solo el collider
	leftBoundNode->setVisible(false);

	
	/////////LIMITE DE LA DERECHA/////////////////////
	//////////////////////////////////////////////////
	size = Ogre::Vector3::ZERO;   // size of the box
	Ogre::Entity* RightBoundEnt = m_pSceneMgr->
			createEntity("RightBoundEnt",
			Ogre::SceneManager::PT_CUBE);
	
	Ogre::SceneNode* rightBoundNode = m_pSceneMgr->getRootSceneNode()->
			createChildSceneNode("rightBoundNode");
		
		
	boundingB = RightBoundEnt->getBoundingBox();
	size = boundingB.getSize(); size /= 2.0f; // only the half needed
	size *= 0.96f;
		
	rightBoundNode->attachObject(RightBoundEnt);
	rightBoundNode->setPosition(12,0,0);
	rightBoundNode->setScale(0.02,50,10);
	barraPos = rightBoundNode->getPosition();

	size = Ogre::Vector3(size.x * 0.02, size.y*50 , size.z*10); 

	//AHORA LA PARTE DE FISICAS
	sceneBoxShape =	new OgreBulletCollisions::BoxCollisionShape(size);
	// and the Bullet rigid body
	OgreBulletDynamics::RigidBody *rightBody = new OgreBulletDynamics::RigidBody(
		"RightBoundBody" + Ogre::StringConverter::toString
			(m_pPhysicsMgr->m_NumEntitiesInstanced),
				m_pPhysicsMgr->m_pWorld);

	rightBody->setShape(rightBoundNode, sceneBoxShape,
						   0,         // dynamic body restitution
						   0,         // dynamic body friction
						   0,          // dynamic bodymass
						   barraPos,      // starting position of the box
						   Ogre::Quaternion(0,0,0,1));// orientation of the box
	             
	// push the created objects to the deques
	m_pPhysicsMgr->m_Shapes.push_back(sceneBoxShape);           

	m_pPhysicsMgr->m_NumEntitiesInstanced++;

	Bound* bound3 = new Bound(rightBoundNode,rightBody);
	m_Bounds.push_back(bound3);
	rightBody->getBulletRigidBody()->setUserPointer(bound3);	
	//no necesito que se vea, solo el collider
	rightBoundNode->setVisible(false);
	
	/////////LIMITE DEL SUELO/////////////////////////
	//////////////////////////////////////////////////
	Ogre::Entity* floorBoundEnt = m_pSceneMgr->
			createEntity("floorBoundEnt",
			Ogre::SceneManager::PT_CUBE);
		// Give the plane a texture
	floorBoundEnt->setMaterialName("Examples/BumpyMetal");
	Ogre::SceneNode* floorBoundNode = m_pSceneMgr->getRootSceneNode()->
			createChildSceneNode("floorBoundNode");
		
	boundingB = floorBoundEnt->getBoundingBox();
	size = boundingB.getSize(); size /= 2.0f; // only the half needed
	size *= 0.96f;
		
	floorBoundNode->attachObject(floorBoundEnt);
	floorBoundNode->setPosition(0,-5,0);
	floorBoundNode->setScale(50,0.1,2);
	barraPos = floorBoundNode->getPosition();

	size = Ogre::Vector3(size.x * 50, size.y*0.1 , size.z*2); 

	//AHORA LA PARTE DE FISICAS
	sceneBoxShape = new OgreBulletCollisions::BoxCollisionShape(size);
	// and the Bullet rigid body
	OgreBulletDynamics::RigidBody *floorBody = new OgreBulletDynamics::RigidBody(
		"floorBoundBody" + Ogre::StringConverter::toString
			(m_pPhysicsMgr->m_NumEntitiesInstanced),
				m_pPhysicsMgr->m_pWorld);

	floorBody->setShape(floorBoundNode, sceneBoxShape,
						   0,         // dynamic body restitution
						   0,         // dynamic body friction
						   0,          // dynamic bodymass
						   barraPos,      // starting position of the box
						   Ogre::Quaternion(0,0,0,1));// orientation of the box
	             
	// push the created objects to the deques
	m_pPhysicsMgr->m_Shapes.push_back(sceneBoxShape);

	m_pPhysicsMgr->m_NumEntitiesInstanced++;

	Bound* bound4 = new Bound(floorBoundNode,floorBody);
	m_Bounds.push_back(bound4);
	floorBody->getBulletRigidBody()->setUserPointer(bound4);
	//no necesito que se vea, solo el collider
	floorBoundNode->setVisible(false);
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void LevelManager::createLevel(){
		
	srand(time(0));
	int randomPos = 0;
	//CREAR LAS BARRAS*********************************
	for(int i = 0; i < 9; i++){
		
		Ogre::Vector3 size = Ogre::Vector3::ZERO;   // size of the box

		Ogre::SceneNode* ProvisionalNode =
			m_pSceneMgr->getSceneNode("ObstacleBar#"+Ogre::StringConverter::toString(i));
		Ogre::Entity* provisionalEnt =
			m_pSceneMgr->getEntity("ObstacleBar#"+Ogre::StringConverter::toString(i));

		Ogre::AxisAlignedBox boundingB = provisionalEnt->getBoundingBox();
		size = boundingB.getSize(); 
		size /= 2.0f; // only the half needed
		size *= 0.96f;
		
		Ogre::Vector3 barraPos = ProvisionalNode->getPosition();
		
		//AHORA LA PARTE DE FISICAS
		  OgreBulletCollisions::BoxCollisionShape *sceneBoxShape = 
			  new OgreBulletCollisions::BoxCollisionShape(size);
		 // and the Bullet rigid body
		 OgreBulletDynamics::RigidBody *defaultBody = new OgreBulletDynamics::RigidBody(
			 "barra" + Ogre::StringConverter::toString
			   (m_pPhysicsMgr->m_NumEntitiesInstanced),
			   m_pPhysicsMgr->m_pWorld);

		 defaultBody->setShape(ProvisionalNode, sceneBoxShape,
						   0,         // dynamic body restitution
						   0,         // dynamic body friction
						   0,          // dynamic bodymass
						   barraPos,      // starting position of the box
						   Ogre::Quaternion(0,0,0,1));// orientation of the box
	             
		// push the created objects to the deques
		 m_pPhysicsMgr->m_Shapes.push_back(sceneBoxShape);
		 m_pPhysicsMgr->m_Bodies.push_back(defaultBody);            

		 m_pPhysicsMgr->m_NumEntitiesInstanced++;

		//crear objecto ObstacleBar
		ObstacleBar* obstacle = new ObstacleBar(ProvisionalNode, defaultBody,
			"Obstaculo");
		pushObstacle(obstacle);
		
		defaultBody->getBulletRigidBody()->setUserPointer(obstacle);
		
		randomPos = rand() % 10; //maxima posicion hardcodeada 
		obstacle->shrink(randomPos); //iniciar  a una altura aleatoria cada barra
	}

	createBounds();
	
	/*
	Ogre::SceneNode* particulaNodo = m_pSceneMgr->getRootSceneNode()->
		createChildSceneNode("particulaNodo");
	Ogre::ParticleSystem* particle =  m_pSceneMgr->createParticleSystem
		("particulatest", "DigitalExplosionSmall");
	particulaNodo->attachObject(particle);
	particulaNodo->setPosition(0,0,0);
	*/ 
}
//|||||||||||||||||||||||||||||||||||||||||||||||


void LevelManager::destroyProyectile(Proyectile *proyectile){
	proyectile->destroy(m_pSceneMgr);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void LevelManager::resetLevel(){
	
	srand(time(0));
	int randomPos = 0;

	std::deque<ObstacleBar*>::iterator it 
		= m_Obstacles.begin();
	while (m_Obstacles.end() != it)
	{   
		(*it)->reset();
		randomPos = rand() % 10; //maxima posicion hardcodeada 
		(*it)->shrink(randomPos);
		
		it++;
	} 
	
}