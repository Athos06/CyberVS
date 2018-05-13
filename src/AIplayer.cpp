#include "Player.h"
#include "AIplayer.h"

#include <Ogre.h>
#include "GameObject.h"
#include "PhysicsManager.h"
#include "Weapon.h"
#include "Euler.h"
#include "GameManager.h"

#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"		
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"

#include "Proyectile.h"
#include "ExplosionsManager.h"

#include "AudioManager.h"

#include "AIinput.h"
#include "GameManager.h"

AIplayer::AIplayer() : Player(){
	m_pAIinput		= 0;

}

AIplayer::AIplayer(Ogre::SceneNode* node, int nPlayer, int AIdiff) : Player(node, nPlayer) {
	m_pAIinput = 0;
	m_difficulty = AIdiff;
}

AIplayer::~AIplayer(){
	
}

void AIplayer::createPlayer(Ogre::SceneManager* sceneMgr){
	m_pAIinput = new AIinput(); 
	m_pAIinput->initAI(m_difficulty);

	m_pWeapon = new Weapon(this);
	m_pWeapon->createWeapon();

	m_InitialPos = m_pNode->getPosition();

	//AHORA LA PARTE DE FISICAS
	Ogre::Euler orientation = orientation.fromQuaternion(m_pNode->getOrientation());		
	Ogre::Vector3 pos = m_pNode->getPosition();

	Ogre::Entity *entity =  sceneMgr->getEntity(m_pNode->getName()+"Turret");

	if(m_NumberPlayer == 1)
		entity->setMaterialName("TurretMaterialGreen");

	OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter =
		new OgreBulletCollisions::StaticMeshToShapeConverter(entity);
    OgreBulletCollisions::CollisionShape *bodyShape = trimeshConverter->createConvex();
	delete trimeshConverter;

	 // and the Bullet rigid body
	OgreBulletDynamics::RigidBody *defaultBody = new OgreBulletDynamics::RigidBody(
		 "player" + Ogre::StringConverter::toString
		   (PhysicsManager::getSingletonPtr()->m_NumEntitiesInstanced),
		   PhysicsManager::getSingletonPtr()->m_pWorld);

	defaultBody->setShape(m_pNode, bodyShape,
					   0,         // dynamic body restitution
					   0,         // dynamic body friction
					   0,          // dynamic bodymass
					   pos,      // starting position of the box
					   orientation);// orientation of the box
             
	// push the created objects to the deques
	PhysicsManager::getSingletonPtr()->m_Shapes.push_back(bodyShape);
	PhysicsManager::getSingletonPtr()->m_Bodies.push_back(defaultBody);            
	
	m_pRigidBody = defaultBody;

	PhysicsManager::getSingletonPtr()->m_NumEntitiesInstanced++;
	
	defaultBody->getBulletRigidBody()->setUserPointer(this);

	
	Ogre::SceneNode* particleNode = m_pNode->createChildSceneNode(m_pNode->getName()+"particle");
	
	m_pParticle = sceneMgr->createParticleSystem("particlePlayer"+
		Ogre::StringConverter::toString(m_NumberPlayer)+"Particle", "digitalCascade"+
		Ogre::StringConverter::toString(m_NumberPlayer)); 
	particleNode->attachObject(m_pParticle);
	
}

Ogre::Quaternion AIplayer::getCannonRotation(){
	return m_pWeapon->m_pNode->_getDerivedOrientation();
}


void AIplayer::update(double timeSinceLastFrame){

	if(!( GameManager::getSingletonPtr()->isGettingReady() 
	|| GameManager::getSingletonPtr()->isRoundTransition() 
	|| GameManager::getSingletonPtr()->isEnding() ) ){	
		
		m_pAIinput->getInput(this, timeSinceLastFrame);
		
		if(m_ShootActive){
			m_ShootPressedTime += timeSinceLastFrame/1000; //pasar de ms a s
			if(m_ShootPressedTime >= m_MaxShootTime){
				releaseShot();
			}
		}
	}
	else{
		m_ShootActive = false;
		m_ShootPressedTime = 0;
	}

}

void AIplayer::score(){
	if(!( GameManager::getSingletonPtr()->isGettingReady() 
	|| GameManager::getSingletonPtr()->isRoundTransition() 
	|| GameManager::getSingletonPtr()->isEnding() ) ){
		m_Score++;
		GameManager::getSingletonPtr()->updateScore();
		
		m_pAIinput->resetInput();
	}
}

void AIplayer::getHit(){
	//cuando el jugador es alcanzado si tiene algun disparo activo
	//este tambien se destruye
	if(m_pActiveProyectile)
		m_pActiveProyectile->destroy(GameManager::getSingletonPtr()->m_pSceneMgr);

	if (m_pEnemy->getScore() >= GameManager::getSingletonPtr()->getScoreLimit()){
		AudioManager::getSingletonPtr()->playFX("PlayerDie");
		ExplosionsManager::getSingletonPtr()->createExplosion
			(2, "DigitalFinalExplosion" , m_pNode->getPosition() );
		m_pNode->setVisible(false);
	}
	else{
		AudioManager::getSingletonPtr()->playFX("PlayerExplosion");
	}	
	m_pAIinput->resetInput();
}