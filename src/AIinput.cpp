//|||||||||||||||||||||||||||||||||||||||||||||||
#include "AIinput.h"
#include "Player.h"
#include "AIplayer.h"
#include "GameManager.h"
#include "Weapon.h"
#include "Constants.h"
#include "Euler.h"

#include <math.h>  
//|||||||||||||||||||||||||||||||||||||||||||||||

AIinput::AIinput(){
	m_readyToShoot = false;
	m_readyRotation = false;
	m_readyPower = false;

	m_goalRotation = 0;
	m_goalPower = 0;//13.127f;
	
	m_initialError	= BASE_AI_ERROR;
	m_currentError	= m_initialError;
	m_initialAngle	= 0;
	
	m_getAngle = true;
	m_getPower = true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

AIinput::~AIinput(){

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void AIinput::initAI(int difficulty){
	switch(difficulty){
		case AI_EASY:
			m_initialError	*= 4;
			break;
		case AI_NORMAL:
			m_initialError	*= 2;
			break;
		case AI_HARD:
			m_initialError	= 1;
			break;
		default:
			break;
	}
	m_currentError	= m_initialError;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

//utiliza las ecuaciones de tiro parabolico para calcular la fuerza
//dependiendo del angulo
float AIinput::calcForce(AIplayer* player, int angle){
	float r =  abs( player->m_pWeapon->getShotPosition().x - 
		player->m_pEnemy->m_pNode->getPosition().x );

	float h = player->m_pWeapon->getShotPosition().y -
		player->m_pEnemy->m_pNode->getPosition().y;
	float t = sqrt( (h + (r*tan(angle*PI/180))) / 4.9 );
	float forceResult = r/(cos(angle*PI/180)*t);  

	float error = 0;	
	if(m_currentError> 0){
		 
		error = (rand()%m_currentError) - (float)m_currentError/2;

	}
	forceResult += error;
	
	m_currentError--;

	if(m_currentError < 0)
		m_currentError = 0;
	
	return forceResult;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void AIinput::getAngle(){
	m_goalRotation = (rand()%45) + 25;
	if(m_goalRotation > m_initialAngle){
		m_rotDirection = true;
	}
	else{
		m_rotDirection = false;
	}
	m_getAngle = false;

	m_initialAngle = m_goalRotation;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void AIinput::getInput(AIplayer* player, double timeSinceLastFrame){
	float deltaTime = timeSinceLastFrame/1000;
	
	if(m_getAngle){
		getAngle();
	}
	Ogre::Euler angle;

	angle.fromQuaternion( player->getCannonRotation() );
	if(m_rotDirection){
		if(angle.roll().valueDegrees() < m_goalRotation){ 
			player->rotateCannon(CANNON_ROT_SPEED*deltaTime);
			m_readyRotation = false;
			return;
		}
		else{
			m_readyRotation = true;
		}
	}
	if(!m_rotDirection){
		if(angle.roll().valueDegrees() > m_goalRotation){ 
			player->rotateCannon(-CANNON_ROT_SPEED*deltaTime);
			m_readyRotation = false;
			return;
		}
		else{
			m_readyRotation = true;
		}
	}
	if(m_readyRotation){
		if(m_getPower){
			m_goalPower = calcForce(player, m_goalRotation);
			m_getPower = false;
		}
		player->beginShot();
	}
	float m_currentPower =	player->getCurrentShotPower();
	if(m_currentPower >= m_goalPower){
		m_readyPower = true;
		m_getPower = true;
	}
	if(m_readyPower){
		m_readyPower = false;
		player->releaseShot();
		m_getAngle = true;
	}
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void AIinput::resetInput(){
	m_readyToShoot = false;
	m_readyRotation = false;
	m_readyPower = false;

	m_goalRotation = 0;
	m_goalPower = 0;
	
	m_currentError	= m_initialError ;

	m_initialAngle	= 0;
	
	m_getAngle = true;
	m_getPower = true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||
