#include "Bound.h"

Bound::Bound(){
	m_pNode			= 0;
	m_pRigidBody	= 0;
	tag				= "bound";
}

Bound::Bound(Ogre::SceneNode* node, OgreBulletDynamics::RigidBody* rigidbody){
	m_pNode			= node;
	m_pRigidBody	= rigidbody;
	tag				= "bound";
}

Bound::~Bound(){
}