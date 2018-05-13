//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef HUD_H
#define HUD_H
//|||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>
//|||||||||||||||||||||||||||||||||||||||||||||||

class HUD{
public:
	HUD();
	~HUD();
	
	void update();
	void showMessage(bool);
	void setMessage(Ogre::String);
	void hide();
	void show();
private:
	Ogre::OverlayManager* m_pOverlayMgr;
	Ogre::Overlay* m_pMainOverlay;
	Ogre::Overlay* m_pSecondaryOverlay;
};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif
//|||||||||||||||||||||||||||||||||||||||||||||||