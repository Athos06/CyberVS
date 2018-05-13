#ifndef EXPLOSIONS_MGR_H
#define EXPLOSIONS_MGR_H

#include <Ogre.h>
#include <vector>
#include <list>
#include "Explosion.h"
#include "GameObject.h"
class ExplosionsManager : public Ogre::Singleton<ExplosionsManager>{
public:
	static ExplosionsManager& getSingleton();
	static ExplosionsManager* getSingletonPtr();

	ExplosionsManager();
	ExplosionsManager(Ogre::SceneManager*);
	~ExplosionsManager();
	void update(double);
	void createExplosion(float, Ogre::String, Ogre::Vector3);
	void destroyExplosion(Explosion*);//(GameObject*);

	Ogre::SceneManager* m_pSceneMgr;
	int m_NumInstanced; //para crear nombres unicos sin conflicto

	std::vector<Explosion*> m_pExplosionsList;
private:
	bool once;
};

#endif