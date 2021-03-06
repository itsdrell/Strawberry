#include "AudioSystem.hpp"
#include "Engine\Core\Tools\ErrorWarningAssert.hpp"
#include "..\Core\Tools\Command.hpp"

//===============================================================================================
AudioSystem* AudioSystem::s_theAudioSystem = nullptr;

//-----------------------------------------------------------------------------------------------
static void ToggleAudio(Command & command)
{
	UNUSED(command);
	AudioSystem::GetInstance()->ToggleMasterMute();
}

//===============================================================================================
AudioSystem::AudioSystem()
{
	FMOD_RESULT result;
	result = FMOD::System_Create( &m_fmodSystem );
	ValidateResult( result );

	result = m_fmodSystem->init( 512, FMOD_INIT_NORMAL, nullptr );
	ValidateResult( result );

	if(s_theAudioSystem == nullptr)
		s_theAudioSystem = this;

	CommandRegister("mute", "mute", "Toggles audio either on or off", ToggleAudio);
}


//-----------------------------------------------------------------------------------------------
AudioSystem::~AudioSystem()
{
	ReleaseFModAndSounds();
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::ReleaseFModAndSounds()
{
	// I think this is how you free up fmod. Release all sounds we made and then Release the system
	ReleaseSounds();
	
	m_fmodSystem->release();
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::ReleaseSounds()
{
	for (uint i = 0; i < m_registeredSounds.size(); i++)
	{
		m_registeredSounds.at(i)->release();
	}

	m_registeredSounds.clear();
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::Shutdown()
{
	DeleteAllAudioClips();
	
	delete s_theAudioSystem;
	s_theAudioSystem = nullptr;
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::FlushRegisteredSounds()
{
	DeleteAllAudioClips();

	m_registeredSoundIDs.clear();
	
	ReleaseSounds();
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::DeleteAllAudioClips()
{
	for (uint i = 0; i < m_audioClips.size(); i++)
	{
		AudioClip* current = m_audioClips.at(i);

		delete current;
		current = nullptr;
	}

	m_audioClips.clear();
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::BeginFrame()
{
	m_fmodSystem->update();
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::EndFrame()
{
}

//-----------------------------------------------------------------------------------------------
SoundID AudioSystem::CreateOrGetSound(const String & soundFilePath)
{
	String thePath = soundFilePath;

#ifdef EMSCRIPTEN_PORT
	//thePath = String("Run_Win32/" + thePath);
	//PrintLog("Loaded Sound:" + thePath);
#endif

	std::map< std::string, SoundID >::iterator found = m_registeredSoundIDs.find( thePath );
	if( found != m_registeredSoundIDs.end() )
	{
		PrintLog("Found the sound at" + thePath);
		return found->second;
	}
	else
	{
		PrintLog("Creating the sound at" + thePath);
		FMOD::Sound* newSound = nullptr;
		FMOD_RESULT theResult = m_fmodSystem->createSound( thePath.c_str(), FMOD_DEFAULT, nullptr, &newSound );
		ValidateResult(theResult);

		if( newSound )
		{
			SoundID newSoundID = m_registeredSounds.size();
			m_registeredSoundIDs[ thePath ] = newSoundID;
			m_registeredSounds.push_back( newSound );

			AudioClip* newClip = new AudioClip(thePath, newSoundID);
			m_audioClips.push_back(newClip);

			return newSoundID;
		}
	}

	return MISSING_SOUND_ID;
}

//-----------------------------------------------------------------------------------------------
AudioClip* AudioSystem::GetAudioClip(const String& path)
{
	for (uint i = 0; i < m_audioClips.size(); i++)
	{
		AudioClip* current = m_audioClips.at(i);

		if (current->m_path == path)
			return current;
	}

	return nullptr;
}

//-----------------------------------------------------------------------------------------------
AudioClip* AudioSystem::GetAudioClip(SoundID theID)
{
	for (uint i = 0; i < m_audioClips.size(); i++)
	{
		AudioClip* current = m_audioClips.at(i);

		if (current->m_soundID == theID)
			return current;
	}

	return nullptr;
}

//-----------------------------------------------------------------------------------------------
AudioSystem * AudioSystem::GetInstance()
{
	return s_theAudioSystem;
}

//-----------------------------------------------------------------------------------------------
SoundPlaybackID AudioSystem::PlaySound(SoundID soundID, bool isLooped, float volume, float balance, float speed, bool isPaused)
{
	size_t numSounds = m_registeredSounds.size();
	if (soundID < 0 || soundID >= numSounds)
	{
		PrintLog("Sound ID is invalid");
		return MISSING_SOUND_ID;
	}

	FMOD::Sound* sound = m_registeredSounds[ soundID ];
	if (!sound)
	{
		PrintLog("Registered Sound is null");
		return MISSING_SOUND_ID;
	}

	FMOD::Channel* channelAssignedToSound = nullptr;
	FMOD_RESULT theResult = m_fmodSystem->playSound( sound, nullptr, isPaused, &channelAssignedToSound );
	ValidateResult(theResult);
	
	if( channelAssignedToSound )
	{
		int loopCount = isLooped ? -1 : 0;
		unsigned int playbackMode = isLooped ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		float frequency;
		channelAssignedToSound->setMode(playbackMode);
		channelAssignedToSound->getFrequency( &frequency );
		channelAssignedToSound->setFrequency( frequency * speed );
		channelAssignedToSound->setVolume( volume );
		channelAssignedToSound->setPan( balance );
		channelAssignedToSound->setLoopCount( loopCount );
	}

	AudioClip* theClip = GetAudioClip(soundID);
	theClip->m_playbackID = (SoundPlaybackID)channelAssignedToSound;

	return (SoundPlaybackID) channelAssignedToSound;
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::StopSound(SoundPlaybackID soundPlaybackID)
{
	if( soundPlaybackID == MISSING_SOUND_ID )
	{
		PrintLog( "WARNING: attempt to set volume on missing sound playback ID!" );
		return;
	}

	FMOD::Channel* channelAssignedToSound = (FMOD::Channel*) soundPlaybackID;
	channelAssignedToSound->stop();
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::StopAllSounds()
{
	for (uint i = 0; i < m_audioClips.size(); i++)
	{
		AudioClip* current = m_audioClips.at(i);
		StopSound(current->m_playbackID);
	}
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::SetSoundPlaybackVolume(SoundPlaybackID soundPlaybackID, float volume)
{
	// 0 - 1
	if( soundPlaybackID == MISSING_SOUND_ID )
	{
		PrintLog( "WARNING: attempt to set volume on missing sound playback ID!" );
		return;
	}

	FMOD::Channel* channelAssignedToSound = (FMOD::Channel*) soundPlaybackID;
	channelAssignedToSound->setVolume( volume );
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::SetSoundPlaybackBalance(SoundPlaybackID soundPlaybackID, float balance)
{
	// is in -1 to 1
	if( soundPlaybackID == MISSING_SOUND_ID )
	{
		PrintLog( "WARNING: attempt to set balance on missing sound playback ID!" );
		return;
	}

	FMOD::Channel* channelAssignedToSound = (FMOD::Channel*) soundPlaybackID;
	channelAssignedToSound->setPan( balance );
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::SetSoundPlaybackSpeed(SoundPlaybackID soundPlaybackID, float speed)
{
	if( soundPlaybackID == MISSING_SOUND_ID )
	{
		PrintLog( "WARNING: attempt to set speed on missing sound playback ID!" );
		return;
	}

	FMOD::Channel* channelAssignedToSound = (FMOD::Channel*) soundPlaybackID;
	float frequency;
	FMOD::Sound* currentSound = nullptr;
	channelAssignedToSound->getCurrentSound( &currentSound );
	if( !currentSound )
		return;

	int ignored = 0;
	currentSound->getDefaults( &frequency, &ignored );
	channelAssignedToSound->setFrequency( frequency * speed );
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::SetMasterVolume(float volume)
{
	FMOD::ChannelGroup* master;
	m_fmodSystem->getMasterChannelGroup(&master);

	master->setVolume(volume);

	master = nullptr;
}

//-----------------------------------------------------------------------------------------------
float AudioSystem::GetMasterVolume()
{
	FMOD::ChannelGroup* master;
	m_fmodSystem->getMasterChannelGroup(&master);

	float volume = 1.f;

	master->getVolume(&volume);

	master = nullptr;
	return volume;
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::ToggleMasterMute()
{
	m_isMuted = !m_isMuted;

	if (m_isMuted == true)
	{
		m_tempPreviousMasterVolume = GetMasterVolume();
		SetMasterVolume(0.0f);
	}
	else
	{
		SetMasterVolume(m_tempPreviousMasterVolume);
	}
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::ValidateResult(FMOD_RESULT result)
{
	if( result != FMOD_OK )
	{
		PrintLog("Engine/Audio SYSTEM ERROR: Got error result code " + std::to_string((int) result) + " - error codes listed in fmod_common.h\n" );
	}
}

//===============================================================================================
void PlayLoopingSound(std::string path, float volume, float balance, float speed, bool isPaused)
{
	AudioSystem* as = AudioSystem::GetInstance();

	AudioClip* theClip = as->GetAudioClip(path);
	as->PlaySound(theClip->m_soundID, true, volume, balance, speed, isPaused);
}

//-----------------------------------------------------------------------------------------------
void StopSound(std::string path)
{
	AudioSystem* as = AudioSystem::GetInstance();

	AudioClip* theClip = as->GetAudioClip(path);

	// stopping a sound not playing should be safe
	if(theClip != nullptr)
	{
		// could give a warning???

		as->StopSound(theClip->m_playbackID);
	}
}

//-----------------------------------------------------------------------------------------------
void PlayOneShot(std::string path, float volume, float balance, float speed, bool isPaused)
{
	AudioSystem* as = AudioSystem::GetInstance();

	AudioClip* theClip = as->GetAudioClip(path);
	as->PlaySound(theClip->m_soundID, false, volume, balance, speed, isPaused);
}
