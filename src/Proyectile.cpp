#include "Proyectile.h"
#include "PhysicsManager.h"
#include "ExplosionsManager.h"
#include "AudioManager.h"

Proyectile::Proyectile(){
}

Proyectile::~Proyectile(){
	m_pNode		= 0;
	tag			= "";
	m_pOwner = 0;
}

Proyectile::Proyectile(Ogre::SceneNode* node, Ogre::String tagName,
					   OgreBulletDynamics::RigidBody* rbody, Player* owner){
	m_pNode	= node;
	tag = tagName;
	m_pRigidBody = rbody;
	m_pOwner = owner;
	m_pOwner->setActiveProyectile(this);

}

void Proyectile::destroy(Ogre::SceneManager* sceneMgr){
	
	AudioManager::getSingletonPtr()->playFX("ProyectileExplosion");
	ExplosionsManager::getSingletonPtr()->createExplosion
		(1, "DigitalExplosionSmall" , m_pNode->getPosition() );
	//"DigitalFinalExplosion"

	delete PhysicsManager::getSingletonPtr()->m_pWorld->findObject(m_pNode);
	sceneMgr->destroySceneNode(m_pNode);
	
	m_pOwner->setAvailableShot(true);
	
	
	
	delete this;
}