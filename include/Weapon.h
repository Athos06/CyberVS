//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef WEAPON_H
#define WEAPON_H
//|||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>
#include "GameObject.h"
//|||||||||||||||||||||||||||||||||||||||||||||||

class Player;
//|||||||||||||||||||||||||||||||||||||||||||||||

class Weapon : public GameObject{
public:
	Weapon();
	Weapon(Ogre::SceneNode*);
	Weapon(Player*);
	~Weapon();
	
	void resetRotation();
	void createWeapon();
	void shoot(float);
	void rotate(float);

	Ogre::Vector3 getShotPosition(){
		return m_pShotPositionNode->_getDerivedPosition();
	};
private:
	Player* m_pParentPlayer;
	Ogre::SceneNode* m_pShotPositionNode;
};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif
//|||||||||||||||||||||||||||||||||||||||||||||||