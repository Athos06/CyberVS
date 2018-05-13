//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef GUI_STATE_h
#define GUI_STATE_h

//|||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>

class GUIState;

//|||||||||||||||||||||||||||||||||||||||||||||||

class GUIStateListener
{
public:
	GUIStateListener(){};
	virtual ~GUIStateListener(){};

	virtual void manageGUIState(Ogre::String stateName, GUIState* state) = 0;

	virtual GUIState*	findByName(Ogre::String stateName) = 0;
	virtual void		changeGUIState(GUIState *state) = 0;
	virtual bool		pushGUIState(GUIState* state) = 0;
	virtual void		popGUIState() = 0;
	virtual void		pauseGUIState() = 0;
    virtual void        popAllAndPushGUIState(GUIState* state) = 0;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

class GUIState //: public OIS::KeyListener, public OIS::MouseListener
{
public:
	static void create(GUIStateListener* parent, const Ogre::String name){};

	void destroy(){delete this;}

	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual bool pause(){return true;}
	virtual void resume(){};


protected:
	GUIState(){};

	GUIState*	findByName(Ogre::String stateName){return m_pParent->findByName(stateName);}
	void		changeGUIState(GUIState* state){m_pParent->changeGUIState(state);}
	bool		pushGUIState(GUIState* state){return m_pParent->pushGUIState(state);}
	void		popGUIState(){m_pParent->popGUIState();}
	void        popAllAndPushGUIState(GUIState* state){m_pParent->popAllAndPushGUIState(state);}

	GUIStateListener*			m_pParent;

	//Ogre::SceneManager*			m_pSceneMgr;
	//Ogre::FrameEvent            m_FrameEvent;
};



//|||||||||||||||||||||||||||||||||||||||||||||||

#define DECLARE_GUISTATE_CLASS(T)													\
static void create(GUIStateListener* parent, const Ogre::String name)				\
{																					\
	T* myGUIState = new T();														\
	myGUIState->m_pParent = parent;													\
	parent->manageGUIState(name, myGUIState);										\
}

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||