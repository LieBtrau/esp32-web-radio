#pragma once
#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "AudioLibs/AudioKit.h"

class Mp3Player
{
public:
    Mp3Player(const char *path, const char *mime_type);
    ~Mp3Player();
    void begin();
    void play();

private:
    AudioKitStream i2s;                      // final output of decoded stream
    EncodedAudioStream dec;
    StreamCopy copier;
    URLStream url;
    const char *path;
    const char *mime_type;
};