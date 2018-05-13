#include "AudioManager.h"

template <> AudioManager* Ogre::Singleton<AudioManager>::msSingleton = 0;

AudioManager& AudioManager::getSingleton(){
	assert (msSingleton);
	return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

AudioManager* AudioManager::getSingletonPtr(){
	assert (msSingleton);
	return msSingleton;
}

AudioManager::AudioManager(){
	m_pTrackMgr = 0;
	m_pSoundFXMgr = 0;	
}

AudioManager::~AudioManager(){
}

bool AudioManager::initAudio()
{
	if(!initSDL())
		return false;
	loadSounds();
	return true;
}

bool AudioManager::initSDL ()
{
  m_pTrackMgr = new TrackManager();
  m_pSoundFXMgr = new SoundFXManager();

  if (SDL_Init (SDL_INIT_AUDIO) < 0)
    return false;
  // Llamar a  SDL_Quit al terminar.
  atexit (SDL_Quit);

  // Inicializando SDL mixer...
  if (Mix_OpenAudio
      (MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS,
       4096) < 0)
    return false;

  // Llamar a Mix_CloseAudio al terminar.
  atexit (Mix_CloseAudio);


  return true;

}

void AudioManager::loadSounds(){
	m_SoundsFXmap.insert(std::make_pair("ProyectileExplosion", m_pSoundFXMgr->load 
		("fm_synthesis_effect_4_good_for_sci_fi_sounds.ogg") ) );
	m_SoundsFXmap.insert(std::make_pair("PlayerExplosion", m_pSoundFXMgr->load 
		("science_fiction_phaser_gun.ogg") ) );
	m_SoundsFXmap.insert(std::make_pair("PlayerDie", 
		m_pSoundFXMgr->load("sound_design_effect_bright_star.ogg") ) );
}


void AudioManager::unloadMainTrack()
{
	m_pMainTrack->unload ();
  
}


void AudioManager:: playMainTrack(Ogre::String sound)
{
	m_pMainTrack = m_pTrackMgr->load(sound);
		m_pMainTrack->play ();
  
}

void
AudioManager::playFX(Ogre::String fxName)
{

	std::map<Ogre::String, SoundFXPtr>::iterator it;
	it = 	m_SoundsFXmap.find(fxName);
	if(it == m_SoundsFXmap.end()) {
	} 
	else {
	  SoundFXPtr sound = it->second;
		sound->play(); 
	}

}
