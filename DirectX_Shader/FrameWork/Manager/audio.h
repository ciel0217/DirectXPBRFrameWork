#pragma once

#include <xaudio2.h>
#include "../Resources/CGameObject.h"

#define SOUND_SOURCE_MAX (20)

class Audio : public CGameObject3D
{
private:
	static IXAudio2* m_Xaudio;
	static IXAudio2MasteringVoice* m_MasteringVoice;

	IXAudio2SourceVoice* m_SourceVoice[SOUND_SOURCE_MAX];
	BYTE* m_SoundData;

	int						m_Length;
	int						m_PlayLength;

public:
	static void InitMaster();
	static void UninitMaster();

	void Init() {};
	void Uninit();
	void Update() {};
	void Draw() {};

	void Load(const char* FileName);
	int Play(bool Loop = false, float Volume = 1.0f);
	void SetVolume(float Volume, int num = 0);
	void Stop(int num = 0);
	void SetPitch(float pitch, int num = 0);
	void SetPan(float Right = 1.0f, float Left = 1.0f, int num = 0);
	void StopAll();
};
