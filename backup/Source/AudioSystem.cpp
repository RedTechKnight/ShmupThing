#include "AudioSystem.h"

MixerAudio::MixerAudio()
{
    mMusicHandles.clear();
    mMixChunks.clear();
}

MixerAudio::~MixerAudio()
{
    for(auto& m: mMusicHandles)
    {
        Mix_FreeMusic(m.second);
    }
    mMusicHandles.clear();
    for(auto& c: mMixChunks)
    {
        Mix_FreeChunk(c.second);
    }
    mMixChunks.clear();

    for(int i = 0;i < Mix_QuerySpec(0,0,0);i++)
    {
        Mix_CloseAudio();
    }
    Mix_Quit();
}

void MixerAudio::initAudioSubsytems(const uint32_t& initFlags)
{
    if(Mix_Init(initFlags) < 0)
    {
        std::cout<<"Failed to initialise Mixer Audio. Error: "<<Mix_GetError()<<std::endl;
    }
}

void MixerAudio::openAudio(const unsigned int& freq,const uint16_t& format,const int& channels,const unsigned int& chunkSize)
{
    if(Mix_OpenAudio(freq,format,channels,chunkSize) < 0)
    {
        std::cout<<"Failed to open the audio device! Error: "<<Mix_GetError()<<std::endl;
    }
}

void MixerAudio::printAudioSpecs()
{
    int freq = 0;
    uint16_t format = 0;
    int channels = 0;
    int opened = 0;
    opened = Mix_QuerySpec(&freq,&format,&channels);
    std::cout<<"Audio Details:\n Mix_OpenAudio was called "<<opened<<"times."<<std::endl;
    std::cout<<"Frequency: "<<freq<<std::endl<<"Channels: "<<channels<<std::endl<<"Audio Format: ";
    switch(format)
    {
    case AUDIO_U8:
        std::cout<<"Unsigned 8-bit little endian audio"<<std::endl;
        break;
    case AUDIO_U16LSB:
        std::cout<<"Unsigned 16-bit little endian audio"<<std::endl;
        break;
    case AUDIO_U16MSB:
        std::cout <<"Unsigned 16-bit big endian audio"<<std::endl;
        break;
    case AUDIO_S8:
        std::cout <<"Signed 8-bit little endian audio"<<std::endl;
        break;
    case AUDIO_S16LSB:
        std::cout<<"Signed 16-bit little endian audio"<<std::endl;
        break;
    case AUDIO_S16MSB:
        std::cout<<"Signed 16-bit big endian audio"<<std::endl;
        break;
    case AUDIO_S32LSB:
        std::cout<<"Signed 32-bit little endian audio"<<std::endl;
        break;
    case AUDIO_S32MSB:
        std::cout<<"Signed 32-bit big endian audio"<<std::endl;
        break;
    case AUDIO_F32LSB:
        std::cout<<"Floating point 32-bit little endian audio"<<std::endl;
        break;
    case AUDIO_F32MSB:
        std::cout<<"Floating point 32-bit big endian audio"<<std::endl;
        break;
    }

    std::cout<<"Chunk decoders "<<Mix_GetNumChunkDecoders()<<std::endl;
    for(int i = 0; i < Mix_GetNumChunkDecoders();i++)
    {
        std::cout<<"Chunk Decoder "<<i<<" - "<<Mix_GetChunkDecoder(i)<<std::endl;
    }
    std::cout<<"Music decoders "<<Mix_GetNumMusicDecoders()<<std::endl;
    for(int i = 0;i < Mix_GetNumMusicDecoders();i++)
    {
        std::cout<<"Music decoder "<<i<<" - "<<Mix_GetMusicDecoder(i)<<std::endl;
    }

    std::cout <<"Allocated channels "<<Mix_AllocateChannels(-1)<<std::endl;
}

bool MixerAudio::loadMusic(const std::string& filename,const unsigned int& ID)
{
    mMusicHandles[ID] = Mix_LoadMUS(filename.c_str());
    if(!mMusicHandles[ID])
    {
        std::cout<<"Failed to load the music file "<<filename.c_str()<<" Error: "<<Mix_GetError()<<std::endl;
        return false;
    }
    return true;
}

void MixerAudio::playMusic(const unsigned int& ID,const int& loops,const unsigned int& fadeInTime,const double& position)
{
    Mix_FadeInMusicPos(mMusicHandles[ID],loops,fadeInTime,position);
}

void MixerAudio::pauseMusic()
{
    Mix_PauseMusic();
}

void MixerAudio::stopMusic(const unsigned int& fadeOutTime)
{
    Mix_FadeOutMusic(fadeOutTime);
}

bool MixerAudio::musicFading()
{
    return Mix_FadingMusic();
}

bool MixerAudio::musicPaused()
{
    return Mix_PausedMusic();
}

bool MixerAudio::musicPlaying()
{
    return Mix_PlayingMusic();
}

void MixerAudio::allocateChannels(const unsigned int& numChans)
{
    std::cout<<"Allocated channels "<<Mix_AllocateChannels(numChans)<<std::endl;
    mAllocatedChannels = numChans;
}

bool MixerAudio::loadMixChunk(const std::string& filename,const unsigned int& ID)
{
    mMixChunks[ID] = Mix_LoadWAV(filename.c_str());
    if(!mMixChunks[ID])
    {
        std::cout<<"Failed to load the audio file "<<filename<<" Error: "<<Mix_GetError()<<std::endl;
        return false;
    }
    return true;
}

void MixerAudio::playChannel(const unsigned int& ID,const int& chan,const int& loops,const int& length,const int& fadeIn)
{

        Mix_FadeInChannelTimed(chan,mMixChunks[ID],loops,fadeIn,length);
}


void MixerAudio::pauseChannel(const int& channel)
{
    Mix_Pause(channel);
}

void MixerAudio::stopChannel(const int& channel)
{
    Mix_HaltChannel(channel);
}

bool MixerAudio::channelPlaying(const int& channel)
{
    if(channel > -1)
    {
        return Mix_Playing(channel);
    }
    else if(channel == -1)
    {
        std::cout<<Mix_Playing(channel)<<" channels are playing"<<std::endl;
        if(Mix_Playing(channel) > 0)
            return true;
    }
    return false;
}

bool MixerAudio::channelPaused(const int& channel)
{
        if(channel > -1)
    {
        return Mix_Paused(channel);
    }
    else if(channel == -1)
    {
        std::cout<<Mix_Paused(channel)<<" channels are paused"<<std::endl;
        if(Mix_Paused(channel) > 0)
            return true;
    }
    return false;
}

bool MixerAudio::channelFading(const int& channel)
{
        if(channel > -1)
    {
        return Mix_Fading(channel) ? true : false;
    }
    else if(channel == -1)
    {
        std::cout<<Mix_Fading(channel)<<" channels are fading out"<<std::endl;
        return Mix_Fading(channel) ? true : false;
    }
    return false;
}

void MixerAudio::loadAudioFiles(const std::string& filename)
{
    std::ifstream infile(filename,std::ios::in);

    while(infile.good())
    {
        std::string value;
        infile >> value;
        if(value == "sound")
        {
            std::string audioFile;
            unsigned int ID;
            infile >> audioFile;
            infile >> ID;
            loadMixChunk(audioFile,ID);
        }
    }
}
