#include "AacPlayer.h"

AacPlayer::AacPlayer(AudioKitStream* i2s, URLStream* url, const char *path, const char *mime_type)
    : dec(i2s, new AACDecoderHelix()),copier(dec, *url), pUrl(url), path(path), mime_type(mime_type)
{
}

AacPlayer::~AacPlayer()
{
    pUrl->flush();
    dec.flush();
    copier.end();
    pUrl->end();
    dec.end();
}

void AacPlayer::begin()
{
    dec.begin();
    pUrl->begin(path, mime_type);
}

void AacPlayer::play()
{
    copier.copy();
}