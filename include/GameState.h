//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef GAME_STATE_h
#define GAME_STATE_h

//|||||||||||||||||||||||||||||||||||||||||||||||


#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>

#include "OgreBulletDynamicsRigidBody.h"				 // for OgreBullet
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h" // for static planes
#include "Shapes/OgreBulletCollisionsBoxShape.h"		 // for Boxes

#include <btBulletCollisionCommon.h>

#include "AppState.h"
#include "DotSceneLoader.h"

#include "PhysicsManager.h"

#include "LevelManager.h"

#include "Player.h"
#include "GameManager.h"

#include "ExplosionsManager.h"

class GameState : public AppState
{
public:
	GameState();
	~GameState();

	DECLARE_APPSTATE_CLASS(GameState)

	void enter();
	void createScene();
	void exit();
	bool pause();
	void resume();

	void getInput();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	void update(double timeSinceLastFrame);
private:	
	bool	m_bQuit;
		
	PhysicsManager* m_pPhysicsMgr;
	LevelManager* m_pLevelMgr;
	GameManager* m_pGameMgr;	
	ExplosionsManager* m_pExplosionsMgr;
};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||