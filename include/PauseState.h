//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef PAUSE_STATE_h
#define PAUSE_STATE_h

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

class GUIManager;

class PauseState : public AppState
{
public:
    PauseState();

    DECLARE_APPSTATE_CLASS(PauseState)

    void enter();
    void createScene();
    void exit();

    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);

    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

    void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);

	void update(double timeSinceLastFrame);

private:
	Ogre::Rectangle2D* m_pBackground;
	Ogre::SceneNode* m_pNodeBg;
    bool m_bQuit;
    bool m_bQuestionActive;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||