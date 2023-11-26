#pragma once
#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "AudioLibs/AudioKit.h"

class Mp3Player
{
public:
    Mp3Player(AudioKitStream* i2s, URLStream* url, const char *path, const char *mime_type);
    ~Mp3Player();
    void begin();
    void play();

private:
    EncodedAudioStream dec;
    StreamCopy copier;
    URLStream* pUrl;
    const char *path;
    const char *mime_type;
};