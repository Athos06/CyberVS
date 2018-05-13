#include "Explosion.h"
#include "ExplosionsManager.h"


Explosion::Explosion() {	
	m_AliveTime = 0;
	m_LifeTime = 0;
	m_pNode = 0;
	tag = "particle";
}

Explosion::Explosion(float lifeTime, Ogre::String templateName,  
					 Ogre::Vector3 position){	
	once = false;
	tag = "particle";
	m_AliveTime = 0;
	m_LifeTime = lifeTime;
	Ogre::SceneManager* sceneMgr =	ExplosionsManager::getSingletonPtr()
		->m_pSceneMgr;
	
	m_pNode = sceneMgr->getRootSceneNode()->createChildSceneNode(
		"nodeParticle"+Ogre::StringConverter::toString(
		ExplosionsManager::getSingletonPtr()->m_NumInstanced) );

	m_pParticle =  sceneMgr->createParticleSystem
		("particleParticle"+Ogre::StringConverter::toString(
		ExplosionsManager::getSingletonPtr()->m_NumInstanced), templateName);
	m_pNode->attachObject(m_pParticle);
	m_pNode->setPosition(position);

	ExplosionsManager::getSingletonPtr()->m_NumInstanced++;
	
}




Explosion::~Explosion(){
	//delete m_pParticle;
	//delete m_pNode;
	
	ExplosionsManager::getSingletonPtr()
		->m_pSceneMgr->destroyParticleSystem(m_pParticle);
	m_pNode->detachAllObjects();
	ExplosionsManager::getSingletonPtr()
		->m_pSceneMgr->destroySceneNode(m_pNode);
	
	m_pParticle = 0;
	m_pNode = 0;
}

void Explosion::update(double timeSinceLastFrame){
	m_AliveTime += (timeSinceLastFrame/1000);
	Explosion* aux = this;
	if( m_AliveTime >= m_LifeTime  ){
		if(!once){
			once = true;
			ExplosionsManager::getSingletonPtr()->destroyExplosion(aux);
		}
	}
}

void Explosion::destroy(){
	
	
}


