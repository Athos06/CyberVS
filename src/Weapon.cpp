#include "Weapon.h"
#include <Ogre.h>
#include "OgreBulletDynamicsRigidBody.h"				 // for OgreBullet
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h" // for static planes
#include "Shapes/OgreBulletCollisionsBoxShape.h"		 // for Boxes
#include "Shapes/OgreBulletCollisionsSphereShape.h"	

#include <btBulletCollisionCommon.h>

#include "Euler.h"

#include "PhysicsManager.h"
#include "GameManager.h"
#include "Proyectile.h"
#include "Player.h"

#include "Constants.h"

Weapon::Weapon(){
	m_pNode				= 0;
	tag					= "weapon";

	m_pParentPlayer		= 0;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

Weapon::Weapon(Ogre::SceneNode* node){
	m_pNode				= node;
	tag					= "weapon";

	m_pParentPlayer		= 0;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

Weapon::Weapon(Player* parent){
	m_pNode				= 0;
	tag					= "weapon";

	m_pParentPlayer		= parent;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

Weapon::~Weapon(){
	//no eliminar nodos a saco
	//delete m_pNode;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void Weapon::createWeapon(){

	/*
	m_pNode = GameManager::getSingletonPtr()->m_pSceneMgr->
		getRootSceneNode()->createChildSceneNode( "nodeWeapon1"+
			m_pParentPlayer->m_pNode->getName());
	*/

	m_pNode = GameManager::getSingletonPtr()->m_pSceneMgr->getSceneNode(m_pParentPlayer->
		m_pNode->getName()+"Weapon"); 
		//m_pParentPlayer->m_pNode->getChild(m_pParentPlayer->
		//m_pNode->getName()+"Weapon");
	Ogre::Entity *entity =  GameManager::getSingletonPtr()->
		m_pSceneMgr->createEntity(m_pNode->getName(), "cannon.mesh");
    //entity->setMaterialName("Examples/RustySteel");
	m_pNode->attachObject(entity);
	m_pShotPositionNode = m_pNode->createChildSceneNode(m_pNode->getName()+"Shot");
	m_pShotPositionNode->translate(0.5,0,0);

	//Ogre::Vector3 position = m_pParentPlayer->m_pNode->getPosition();
	//position.y += 3;
	//m_pNode->setPosition(position);
	//m_pNode->scale(0.1f, 0.02f, 0.05f)
	//Ogre::Euler angle; 
	//angle.setYaw(Ogre::Degree(180)); esto no tocar
	//angle = angle.fromQuaternion(m_pParentPlayer->m_pNode->getOrientation() );
	//m_pNode->setOrientation(angle);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void Weapon::resetRotation(){
	
	Ogre::Euler angle;
	angle.fromQuaternion( Ogre::Quaternion::ZERO );
	m_pNode->setOrientation(angle);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void Weapon::shoot(float shotForce){
	

    Ogre::Vector3 position = m_pShotPositionNode->_getDerivedPosition();
	position.y += 1;
    
	Ogre::Entity *entity =  GameManager::getSingletonPtr()->m_pSceneMgr->
		createEntity("Shot" + Ogre::StringConverter::toString(
		PhysicsManager::getSingletonPtr()->m_NumEntitiesInstanced),"shot.mesh");            
     
	//obtener el angulo de disparo
	Ogre::Euler angle; 
	angle = angle.fromQuaternion(m_pNode->_getDerivedOrientation() );
	//obtener el vector de la direccion del disparo
	Ogre::Vector3 direction = angle.right();

     Ogre::SceneNode *node =  GameManager::getSingletonPtr()->
		 m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
     node->attachObject(entity);
	 node->setPosition(position);
	 
		    
	//LA PARTE DE CREACION DEL COLLIDER Y RIGIDBODY
	OgreBulletCollisions::SphereCollisionShape *ballShape = new OgreBulletCollisions::SphereCollisionShape(0.3); 
     // and the Bullet rigid body
     OgreBulletDynamics::RigidBody *defaultBody = new OgreBulletDynamics::RigidBody(
           "shotRigidBody" + Ogre::StringConverter::toString
		   (PhysicsManager::getSingletonPtr()->m_NumEntitiesInstanced),
           PhysicsManager::getSingletonPtr()->m_pWorld, COL_PROYECTILE, proyectileCollidesWith);

     defaultBody->setShape(node, ballShape,
                       0.6f,         // dynamic body restitution
                       0.6f,         // dynamic body friction
                       1.0f,          // dynamic bodymass
                       position,      // starting position 
					   Ogre::Quaternion(0,0,0,1));// orientation 
     //asignar la fuerza al rigidbody
	 defaultBody->setLinearVelocity( direction * shotForce ); 
     
	 // push the created objects to the deques
	PhysicsManager::getSingletonPtr()->m_Shapes.push_back(ballShape);
    //NO AÑADIR EL RIGIDBODY A LA COLA pues tendre el puntero guardado
	//en el proyectil y me encargo de manejarlo ahi
	
	 PhysicsManager::getSingletonPtr()->m_NumEntitiesInstanced++;

	//crear el objeto proyectil
	Proyectile* shot = new Proyectile(node, "Proyectile", defaultBody, m_pParentPlayer);
	defaultBody->getBulletRigidBody()->setUserPointer(shot);

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void Weapon::rotate(float rotation){
	Ogre::Euler previousRot;
	Ogre::Euler angle;
	previousRot.fromQuaternion( m_pNode->getOrientation() );
	angle.fromQuaternion( m_pNode->getOrientation() );
	angle.rotate(Ogre::Degree(0), Ogre::Degree(0), 
		Ogre::Degree(rotation));

	//limite del angulo en el que puede apuntar
	if((angle.roll().valueDegrees()>90) || (angle.roll().valueDegrees() < 0)){
		m_pNode->setOrientation(previousRot);	
	}
	else{
		m_pNode->setOrientation(angle);
	}
}
//|||||||||||||||||||||||||||||||||||||||||||||||
