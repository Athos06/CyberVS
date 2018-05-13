//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef	GAMEOVER_STATE_H
#define GAMEOVER_STATE_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

class GUIManager;

class GameOverState : public AppState
{
public:
    GameOverState();

    DECLARE_APPSTATE_CLASS(GameOverState)

    void enter();
    void createScene();
    void exit();

    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);

    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

    void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);
	bool onExitButtonGame(const CEGUI::EventArgs &args);
    
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