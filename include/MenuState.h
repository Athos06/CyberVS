//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef MENU_STATE_h
#define MENU_STATE_h

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

class MenuState : public AppState
{
public:
    MenuState();

	DECLARE_APPSTATE_CLASS(MenuState)

	void enter();
	void createScene();
	void exit();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	void update(double timeSinceLastFrame);

private:
	bool m_bQuit;
	Ogre::SceneNode* m_pCamNode;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||