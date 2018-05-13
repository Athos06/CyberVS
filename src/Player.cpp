//|||||||||||||||||||||||||||||||||||||||||||||||
#include "Player.h"

#include <Ogre.h>
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
//|||||||||||||||||||||||||||||||||||||||||||||||

Player::Player() :	m_MaxShootTime(2), m_MinShotForce(10), m_MaxShotForce(40){
	m_pNode				= 0;
	tag					="Player";

	m_ShootActive		= false;
	m_ShootPressedTime	= 0;
	
	m_pEnemy			= 0;

	m_ShotForce			= 0;
	
	m_pWeapon			= 0;

	mbFirstCollision	= 0;

	m_Score				= 0;

	m_pParticle			= 0;

	m_pActiveProyectile	= 0;
	m_ShotAvailable = true;

	m_NumberPlayer = 0;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

Player::Player(Ogre::SceneNode* node , int NumberPlayer):m_MaxShootTime(2), 
														m_MinShotForce(8),
														m_MaxShotForce(25){
	m_pNode				= node;
	tag					= "Player";

	m_ShootActive		= false;
	m_ShootPressedTime	= 0;

	m_pEnemy			= 0;

	m_ShotForce			= 0;

	m_pWeapon			= 0;

	mbFirstCollision	= 0;

	m_Score				= 0;

	m_pParticle			= 0;

	m_pActiveProyectile	= 0;

	m_ShotAvailable = true;

	m_NumberPlayer = NumberPlayer;

}
//|||||||||||||||||||||||||||||||||||||||||||||||

Player::~Player(){
	delete m_pWeapon;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void Player::setAvailableShot(bool available){ 
	m_ShotAvailable = available; 
	m_pActiveProyectile = 0;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void Player::resetPlayer(){
	m_ShootActive		= false;
	m_ShootPressedTime	= 0;
	m_ShotForce			= 0;
	m_Score				= 0;
	m_ShotAvailable = true;
	m_pActiveProyectile = 0;
	m_pNode->setVisible(true);
	
	resetPosition();

}
//|||||||||||||||||||||||||||||||||||||||||||||||
void Player::update(double timeSinceLastFrame){
	if(!( GameManager::getSingletonPtr()->isGettingReady() 
	|| GameManager::getSingletonPtr()->isRoundTransition() 
	|| GameManager::getSingletonPtr()->isEnding() ) ){	
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
//|||||||||||||||||||||||||||||||||||||||||||||||

void Player::createPlayer(Ogre::SceneManager* sceneMgr){
		
	m_pWeapon = new Weapon(this);
	m_pWeapon->createWeapon();

	m_InitialPos = m_pNode->getPosition();

	//AHORA LA PARTE DE FISICAS
	Ogre::Euler orientation = orientation.fromQuaternion(m_pNode->getOrientation());		
	Ogre::Vector3 pos = m_pNode->getPosition();

	Ogre::Entity *entity =  sceneMgr->getEntity(m_pNode->getName()+"Turret");

	if(m_NumberPlayer == 1){
		entity->setMaterialName("TurretMaterialGreen");
	}

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

	
	Ogre::SceneNode* particleNode = m_pNode->createChildSceneNode
		(m_pNode->getName()+"particle");
	
	

	m_pParticle = sceneMgr->createParticleSystem("particlePlayer"+
		Ogre::StringConverter::toString(m_NumberPlayer)+"Particle", "digitalCascade"+
		Ogre::StringConverter::toString(m_NumberPlayer)); 
	particleNode->attachObject(m_pParticle);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void Player::getHit(){
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
	//llamar a la logica tras recibir impacto
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void Player::rotateCannon(float rotation){
	//no permitir rotar cuando se esta cargando disparo
	if(!m_ShootActive)
		m_pWeapon->rotate(rotation);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

//activar el disparo
void Player::beginShot(){
	if(!m_ShotAvailable || m_ShootActive)
		return;
	if(!( GameManager::getSingletonPtr()->isGettingReady() 
	|| GameManager::getSingletonPtr()->isRoundTransition() 
	|| GameManager::getSingletonPtr()->isEnding() ) ){	
		m_ShootActive = true;
		m_ShootPressedTime = 0;
	}
}
//|||||||||||||||||||||||||||||||||||||||||||||||

//realizar disparo
void Player::releaseShot(){

	if(!m_ShotAvailable)
		return;
	if(!( GameManager::getSingletonPtr()->isGettingReady() 
	|| GameManager::getSingletonPtr()->isRoundTransition() 
	|| GameManager::getSingletonPtr()->isEnding() ) ){			//calcular la potencia de disparo a partir del tiempo que presionamos espacio	
		if(m_ShootActive){
			m_ShotForce = m_MinShotForce + (m_ShootPressedTime * m_MaxShotForce)/(m_MaxShootTime);
			//resetear disparo
			m_ShootActive = false;
			m_ShotAvailable = false;
			m_ShootPressedTime = 0;
			//realizar el disparo
			m_pWeapon->shoot(m_ShotForce);
		}
	}
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void Player::resetPosition(){
	float randomPosX = 0;
	float randomPosY = 0;

	//( (float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
	//randomPosY = (  (float(rand())/float(RAND_MAX))*(2+2) ) -2;
	//randomPosX = ( (float(rand())/ float(RAND_MAX))* (1+1) ) -1;

	randomPosY = float((rand()%400)-200) / 100;
	randomPosX = float((rand()%200)-100) / 100;

	
	///////////////////////////////TESTEO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	m_pNode->setPosition(m_InitialPos);
	m_pNode->translate(randomPosX, randomPosY, 0);

	
	//mover tambien el rigidbody	
	Ogre::Vector3 position = m_pNode->getPosition(); //Have to be initialized

	btTransform transform; //Declaration of the btTransform
	transform.setIdentity(); //This function put the variable of the object to default. The ctor of btTransform doesnt do it.
	transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(position)); 
	   //Set the new position/origin
	m_pRigidBody->getBulletRigidBody()->setWorldTransform(transform); 
	
	//resetea la rotacion del cañon
	m_pWeapon->resetRotation();

	ExplosionsManager::getSingletonPtr()->createExplosion
		(1, "DigitalTeleport", m_pNode->getPosition() );

	

}


//|||||||||||||||||||||||||||||||||||||||||||||||

void Player::score(){
	if(!( GameManager::getSingletonPtr()->isGettingReady() 
	|| GameManager::getSingletonPtr()->isRoundTransition() 
	|| GameManager::getSingletonPtr()->isEnding() ) ){
		m_Score++;
		GameManager::getSingletonPtr()->updateScore();
	}
}

