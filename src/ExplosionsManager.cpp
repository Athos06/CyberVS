#include "ExplosionsManager.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> ExplosionsManager* Ogre::Singleton<ExplosionsManager>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

ExplosionsManager& ExplosionsManager::getSingleton(){
	assert (msSingleton);
	return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

ExplosionsManager* ExplosionsManager::getSingletonPtr(){
	assert(msSingleton);
	return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||

ExplosionsManager::ExplosionsManager(){
	m_pSceneMgr	= 0;
	m_NumInstanced = 0;
}
//|||||||||||||||||||||||||||||||||||||||

ExplosionsManager::ExplosionsManager(Ogre::SceneManager* sceneMgr){
	m_NumInstanced = 0;
	m_pSceneMgr = sceneMgr;
	once = false;

}
//|||||||||||||||||||||||||||||||||||||||

ExplosionsManager::~ExplosionsManager(){
	once = false;
}
//|||||||||||||||||||||||||||||||||||||||

void ExplosionsManager::update(double timeSinceLastTime){
	
	for(int i = 0; i < m_pExplosionsList.size(); i++){
		m_pExplosionsList[i]->update(timeSinceLastTime);
	}


}

void ExplosionsManager::createExplosion(float lifeTime, Ogre::String templateName
										,  Ogre::Vector3 position){
	
	Explosion* explosion = new Explosion(lifeTime, templateName, position);
	m_pExplosionsList.push_back(explosion);
	int test = 0;
	
}

//||||||||||||||||||||||||||||||||||||||

void ExplosionsManager::destroyExplosion(Explosion* explosion){

	Ogre::String expName = explosion->m_pNode->getName();
	
	for(int i = 0; i < m_pExplosionsList.size(); i++){
		if( m_pExplosionsList[i]->m_pNode->getName() == expName ){
				
				delete m_pExplosionsList[i];
				m_pExplosionsList.erase(m_pExplosionsList.begin()+i);
				return;
			}
	} 
}
