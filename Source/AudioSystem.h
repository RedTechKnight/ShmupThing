#ifndef AUDIOSYSTEM_H_INCLUDED
#define AUDIOSYSTEM_H_INCLUDED
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <deque>
class MixerAudio
{
private:
std::map<unsigned int,Mix_Music*> mMusicHandles;
std::map<unsigned int,Mix_Chunk*> mMixChunks;
unsigned int mAllocatedChannels;
public:
    MixerAudio();
    ~MixerAudio();

    void initAudioSubsytems(const uint32_t& initFlags);
    void openAudio(const unsigned int& frequency,const uint16_t& format,const int& channels,const unsigned int& chunkSize);
    void printAudioSpecs();

    bool loadMusic(const std::string& filename,const unsigned int& ID);
    void playMusic(const unsigned int& ID,const int& loops,const unsigned int& fadeInTime,const double& startPosition);
    void pauseMusic();
    void stopMusic(const unsigned int& fadeOutTime);

    bool musicPlaying();
    bool musicPaused();
    bool musicFading();

    void allocateChannels(const unsigned int& numChannels);
    bool loadMixChunk(const std::string& filename,const unsigned int& ID);
    void playChannel(const unsigned int& ID, const int& channel,const int& loops,const int& playLength,const int& fadeIn);
    void pauseChannel(const int& channel);
    void stopChannel(const int& channel);

    bool channelPlaying(const int& channel);
    bool channelPaused(const int& channel);
    bool channelFading(const int& channel);

    void loadAudioFiles(const std::string& filename);
};


#endif // AUDIOSYSTEM_H_INCLUDED
