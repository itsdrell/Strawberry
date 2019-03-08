#pragma once
#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/ThirdParty/fmod/fmod.hpp"
#include <map>

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================
typedef size_t SoundID;
typedef size_t SoundPlaybackID;
constexpr size_t MISSING_SOUND_ID = (size_t)(-1); // for bad SoundIDs and SoundPlaybackIDs

#define relativePath "Data/Audio/"

//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================
struct AudioClip
{
	AudioClip(std::string name, std::string path, std::string group, uint weight, SoundID id)
		:	m_name(name),
		m_path(path),
		m_group(group),
		m_weight(weight),
		m_soundID(id),
		m_playbackID(MISSING_SOUND_ID) {}


	String				m_name;
	String				m_path;
	String				m_group;
	uint				m_weight;

	SoundID				m_soundID; // used to play a sound

								   // This is used to turn off sound because it is the channel
								   // that is currently assigned to the playing sound.
	SoundPlaybackID		m_playbackID;
};

//====================================================================================
// Classes
//====================================================================================
class AudioSystem
{
public:
	AudioSystem();
	~AudioSystem();
	void ReleaseFModAndSounds();

	static void Shutdown();

public:
	void BeginFrame();
	void EndFrame();

public:
	virtual SoundID CreateOrGetSound( const String& soundFilePath);
	static AudioSystem* GetInstance();

public:
	virtual SoundPlaybackID		PlaySound( SoundID soundID, bool isLooped=false, float volume=1.f, float balance=0.0f, float speed=1.0f, bool isPaused=false );
	virtual void				StopSound( SoundPlaybackID soundPlaybackID );
	virtual void				SetSoundPlaybackVolume( SoundPlaybackID soundPlaybackID, float volume );	// volume is in [0,1]
	virtual void				SetSoundPlaybackBalance( SoundPlaybackID soundPlaybackID, float balance );	// balance is in [-1,1], where 0 is L/R centered
	virtual void				SetSoundPlaybackSpeed( SoundPlaybackID soundPlaybackID, float speed );		// speed is frequency multiplier (1.0 == normal)

public:
	virtual void				SetMasterVolume(float volume);
	virtual float				GetMasterVolume();
	virtual void				ToggleMasterMute();

private:
	virtual void				ValidateResult( FMOD_RESULT result );

private:
	bool							m_isMuted;
	float							m_tempPreviousMasterVolume;

	FMOD::System*					m_fmodSystem;
	std::map< String, SoundID>		m_registeredSoundIDs;
	std::vector< FMOD::Sound*>		m_registeredSounds;

	static AudioSystem*				s_theAudioSystem;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================
