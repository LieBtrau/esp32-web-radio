#pragma once
#include "AudioTools.h"
#include "AudioCodecs/CodecAACHelix.h"
#include "AudioLibs/AudioKit.h"

class AacPlayer
{
public:
    AacPlayer(AudioKitStream* i2s, URLStream* url, const char *path, const char *mime_type);
    ~AacPlayer();
    void begin();
    void play();

private:
    EncodedAudioStream dec;
    StreamCopy copier;
    URLStream* pUrl;
    const char *path;
    const char *mime_type;
};