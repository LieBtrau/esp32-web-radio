#pragma once
#include "AudioTools.h"
#include "AudioCodecs/CodecAACHelix.h"
#include "AudioLibs/AudioKit.h"

class AacPlayer
{
public:
    AacPlayer(const char *path, const char *mime_type);
    ~AacPlayer();
    void begin();
    void play();

private:
    AudioKitStream i2s;
    EncodedAudioStream dec;
    StreamCopy copier;
    URLStream url;
    const char *path;
    const char *mime_type;
};