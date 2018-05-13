//|||||||||||||||||||||||||||||||||||||||||||||||
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
//|||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>
//|||||||||||||||||||||||||||||||||||||||||||||||

class GameObject{
public:
	virtual Ogre::String getTag(){return tag;};
	Ogre::SceneNode* m_pNode;

protected:
	Ogre::String tag;
};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif
//|||||||||||||||||||||||||||||||||||||||||||||||