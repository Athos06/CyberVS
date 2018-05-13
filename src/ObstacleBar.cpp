#include "ObstacleBar.h"
//|||||||||||||||||||||||||||||||||||||||||||||||

ObstacleBar::ObstacleBar(){
	m_pNode = 0;
	tag = "";
	bFirstCollision = false;
	m_pRigidBody = 0;

	m_MaxPos = 10;
	m_Pos = m_MaxPos;

	m_pNode = 0;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

ObstacleBar::~ObstacleBar(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

ObstacleBar::ObstacleBar(Ogre::SceneNode* node, OgreBulletDynamics::RigidBody* rBody,
						 Ogre::String tagName){
	m_pNode = node;
	tag = tagName;
	m_pRigidBody = rBody;
	bFirstCollision = false;

	m_MaxPos = 10;
	m_Pos = m_MaxPos;

	m_InitPos = m_pNode->getPosition();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void ObstacleBar::shrink(int nTimes){

	if(m_Pos > 0){
		if(m_Pos - nTimes >=0){
	// If you use Ogre::Vector3 to store the position
			m_pNode->translate(0, -0.8*nTimes, 0); //0.8 es el valor hardcodeado de cuanto
													//se desplaza la barra al encogerse, (ponerlo en variable)
			Ogre::Vector3 position = m_pNode->getPosition(); //Have to be initialized

			btTransform transform; //Declaration of the btTransform
			transform.setIdentity(); //This function put the variable of the object to default. The ctor of btTransform doesnt do it.
			transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(position)); 
			   //Set the new position/origin
			m_pRigidBody->getBulletRigidBody()->setWorldTransform(transform); 
			m_Pos -= nTimes;
		
		}
	}
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void ObstacleBar::reset(){
	
	m_pNode->setPosition(m_InitPos);
	Ogre::Vector3 position = m_pNode->getPosition(); //Have to be initialized

	btTransform transform; //Declaration of the btTransform
	transform.setIdentity(); //This function put the variable of the object to default. The ctor of btTransform doesnt do it.
	transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(position)); 
	   //Set the new position/origin
	m_pRigidBody->getBulletRigidBody()->setWorldTransform(transform); 
	
	m_Pos = m_MaxPos;
	
}