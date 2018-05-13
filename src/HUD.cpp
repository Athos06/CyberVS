#include "HUD.h"

#include <Ogre.h>
#include "GameManager.h"
#include "Player.h"

HUD::HUD(){

	m_pOverlayMgr = Ogre::OverlayManager::getSingletonPtr();
	m_pMainOverlay = m_pOverlayMgr->getByName("HUD");
	m_pSecondaryOverlay = m_pOverlayMgr->create("ShowImgs");

	//para hacer el calculo de cuanta barra de vida mostrar de acorde a la vida del jugador
	//Ogre::OverlayElement* elem = m_pOverlayMgr->getOverlayElement("PowerBar0BarEmpty");


	Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(
	m_pOverlayMgr->createOverlayElement("Panel", "MessageText") );
	panel->setMetricsMode(Ogre::GMM_RELATIVE); //Ogre::GMM_PIXELS Ogre::GMM_RELATIVE 
	panel->setPosition(0.35, 0.3); //- panel->getHeight()
	panel->setDimensions(0.3, 0.3);
	panel->setMaterialName("ready"); //BaseWhite

	m_pSecondaryOverlay->add2D(panel);
	
	
	m_pMainOverlay->show();	
	m_pSecondaryOverlay->hide();
	//MUY IMPORTANTE: me falta destruir el overlay una vez que salimos del juego al menu
}

HUD::~HUD(){
	//I dunno wut Im doin :D
	m_pOverlayMgr->destroyOverlayElement(m_pOverlayMgr->getOverlayElement
		("MessageText") );
	m_pOverlayMgr->destroy(m_pSecondaryOverlay);}

void HUD::setMessage(Ogre::String message)
{
	Ogre::OverlayElement* elem = m_pOverlayMgr->getOverlayElement("MessageText");
	elem->setMaterialName(message);
}


void HUD::showMessage(bool visible){
	if(visible)
		m_pSecondaryOverlay->show();
	else
		m_pSecondaryOverlay->hide();
}

void HUD::update(){

	
	//actualizar la barra de fuerza de disparo de ambos jugadores
	//primero el jugador 0 y despues el 1
	Ogre::OverlayElement* bar = m_pOverlayMgr->getOverlayElement("PowerBar0BarEmpty");
	int barSize = bar->getWidth();		
	//accedemos a las variables de los jugadores
	float player1PressTime = GameManager::getSingletonPtr()->m_Players[0]->getPressedTime();
	float maxTime = 2; //hardcodeado, cambiar maxShootTime
	float powerRatio = player1PressTime/maxTime;
	float barWidth = barSize * powerRatio;

	Ogre::OverlayElement* powerBar0 = m_pOverlayMgr->getOverlayElement("PowerBar0BarFull");
	
	if(player1PressTime > 0)
	{
		powerBar0->setWidth((int)barWidth); 
	}else
	{
		powerBar0->setWidth(0); 
	}

	//ahora lo mismo con el segundo jugador
	bar = m_pOverlayMgr->getOverlayElement("PowerBar1Empty");
	barSize = bar->getWidth();		
	//accedemos a las variables de los jugadores
	player1PressTime = GameManager::getSingletonPtr()->m_Players[1]->getPressedTime();
	maxTime = 2; //hardcodeado, cambiar maxShootTime
	powerRatio = player1PressTime/maxTime;
	barWidth = barSize * powerRatio;

	Ogre::OverlayElement* powerBar1 = m_pOverlayMgr->getOverlayElement("PowerBar1Full");
	
	if(player1PressTime > 0)
	{
		powerBar1->setWidth((int)barWidth); 
	}else
	{
		powerBar1->setWidth(0); 
	}

	//Ogre::OverlayManager* pOverlayMgr = Ogre::OverlayManager::getSingletonPtr();
	Ogre::OverlayElement* elem = m_pOverlayMgr->getOverlayElement("ScoreText");
	elem->setCaption("SCORE: "+
		Ogre::StringConverter::toString(GameManager::getSingletonPtr()
		->getScore()[0])+ " - " + Ogre::StringConverter::toString
		(GameManager::getSingletonPtr()->getScore()[1] )
		);
}


void HUD::hide(){
	m_pMainOverlay->hide();
	//m_pSecondaryOverlay->hide();
	return;
}	

void HUD::show(){
	m_pMainOverlay->show();
	//m_pSecondaryOverlay->show();
	return;
}	