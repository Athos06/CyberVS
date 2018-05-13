//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef	LEVEL_MANAGER_H
#define LEVEL_MANAGER_H
//|||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>

#include "PhysicsManager.h"
#include "ObstacleBar.h"
#include "Proyectile.h"
#include "Bound.h"
//|||||||||||||||||||||||||||||||||||||||||||||||

class LevelManager : public Ogre::Singleton<LevelManager>{
public:
	static LevelManager& getSingleton();
	static LevelManager* getSingletonPtr();

	LevelManager();
	LevelManager(Ogre::SceneManager*, PhysicsManager*);
	~LevelManager();

	Ogre::SceneNode* findByName(Ogre::String);
	void pushNode(Ogre::SceneNode*);
	void popNode(Ogre::SceneNode*);

	void pushObstacle(ObstacleBar*);
	void popObstacle(ObstacleBar*);

	void createLevel();
	
	void destroyProyectile(Proyectile*);

	void createBounds();
	void resetLevel();

	Ogre::SceneNode* m_Obstacle1;

private:
	Ogre::SceneManager*	m_pSceneMgr;
	PhysicsManager* m_pPhysicsMgr;
	std::deque<ObstacleBar*> m_Obstacles;
	std::deque<Bound*> m_Bounds;
};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif
//|||||||||||||||||||||||||||||||||||||||||||||||
