#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <Ogre.h>

#include "SoundFXManager.h"
#include "TrackManager.h"
#include <map>

class AudioManager : public Ogre::Singleton<AudioManager> {
public:
	static AudioManager& getSingleton();
	static AudioManager* getSingletonPtr();

	AudioManager();
	AudioManager(Ogre::SceneManager*);
	~AudioManager();	
	
	bool initAudio();

	bool initSDL();

	void loadSounds();
	void unloadMainTrack();
	void playMainTrack(Ogre::String);
	void playFX(Ogre::String);
	
	TrackPtr m_pMainTrack;
private:
	TrackManager* m_pTrackMgr;
	SoundFXManager* m_pSoundFXMgr;

	std::map<std::string, SoundFXPtr> m_SoundsFXmap;
	
	
};



#endif