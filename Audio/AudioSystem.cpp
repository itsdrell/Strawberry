#include "AudioSystem.hpp"
#include "Engine\Core\Tools\ErrorWarningAssert.hpp"

//===============================================================================================
AudioSystem* AudioSystem::s_theAudioSystem = nullptr;

//-----------------------------------------------------------------------------------------------
AudioSystem::AudioSystem()
{
	FMOD_RESULT result;
	result = FMOD::System_Create( &m_fmodSystem );
	ValidateResult( result );

	result = m_fmodSystem->init( 512, FMOD_INIT_NORMAL, nullptr );
	ValidateResult( result );

	if(s_theAudioSystem == nullptr)
		s_theAudioSystem = this;
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

	for(uint i = 0; i < m_registeredSounds.size(); i++)
	{
		//TODO figure out how this works :(
		//FMOD_RESULT result = m_registeredSounds.at(i)->release();
	}

	m_registeredSounds.clear();

	m_fmodSystem->release();
}

//-----------------------------------------------------------------------------------------------
void AudioSystem::Shutdown()
{
	delete s_theAudioSystem;
	s_theAudioSystem = nullptr;
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
	thePath = String("Run_Win32/" + thePath);
#endif

	std::map< std::string, SoundID >::iterator found = m_registeredSoundIDs.find( thePath );
	if( found != m_registeredSoundIDs.end() )
	{
		return found->second;
	}
	else
	{
		FMOD::Sound* newSound = nullptr;
		m_fmodSystem->createSound( thePath.c_str(), FMOD_DEFAULT, nullptr, &newSound );
		if( newSound )
		{
			SoundID newSoundID = m_registeredSounds.size();
			m_registeredSoundIDs[ thePath ] = newSoundID;
			m_registeredSounds.push_back( newSound );
			return newSoundID;
		}
	}

	return MISSING_SOUND_ID;
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
	if( soundID < 0 || soundID >= numSounds )
		return MISSING_SOUND_ID;

	FMOD::Sound* sound = m_registeredSounds[ soundID ];
	if( !sound )
		return MISSING_SOUND_ID;

	FMOD::Channel* channelAssignedToSound = nullptr;
	m_fmodSystem->playSound( sound, nullptr, isPaused, &channelAssignedToSound );
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
