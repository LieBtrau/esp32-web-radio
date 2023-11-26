#include "Mp3Player.h"

Mp3Player::Mp3Player(AudioKitStream* i2s, URLStream* url, const char *path, const char *mime_type)
    : dec(i2s, new MP3DecoderHelix()),copier(dec, *url), pUrl(url), path(path), mime_type(mime_type)
{
}

Mp3Player::~Mp3Player()
{
    pUrl->flush();
    dec.flush();
    copier.end();
    pUrl->end();
    dec.end();
}

void Mp3Player::begin()
{
    dec.begin();
    pUrl->begin(path, mime_type);
}

void Mp3Player::play()
{
    copier.copy();
}