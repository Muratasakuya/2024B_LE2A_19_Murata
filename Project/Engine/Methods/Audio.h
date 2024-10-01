#pragma once

//===================================================================*/
//								include
//===================================================================*/
#include "Engine/MyDirectXClass/ComPtr.h"

// directX
#include <xaudio2.h>

// c++
#include <fstream>
#include <cassert>
#include <unordered_map>
#include <string>
#include <filesystem>

// 音声データ
struct SoundData {

	WAVEFORMATEX wfex;
	BYTE* pBuffer;
	uint32_t bufferSize;
};

/*////////////////////////////////////////////////////////////////////////////////
*								Audio Class
////////////////////////////////////////////////////////////////////////////////*/
class Audio {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	Audio() = default;
	~Audio() = default;

	// チャンク
	struct ChunkHeader {

		char id[4];
		int32_t size;
	};

	// RIFFチャンク
	struct RiffHeader {
		ChunkHeader chunk;
		char type[4];
	};

	// FMTチャンク
	struct FormatChunk {

		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};

	void LoadWave(const std::string& filename);

	void PlayWave(const std::string& name, bool loop = false);
	void StopWave(const std::string& name);

	void PauseWave(const std::string& name);
	void ResumeWave(const std::string& name);

	void SetVolume(const std::string& name, float volume);

	bool IsPlayWave(const std::string& name);

	void Initialize();

	void Unload();
	void Finalize();

private:
	//===================================================================*/
	//							private Variables
	//===================================================================*/

	ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_{};

	std::unordered_map<std::string, SoundData> soundData_;
	std::unordered_map<std::string, IXAudio2SourceVoice*> activeVoices_;

private:
	//===================================================================*/
	//							private Functions
	//===================================================================*/

	SoundData LoadWaveInternal(const std::string& filename);
	std::string GetFileNameWithoutExtension(const std::string& filename);
};