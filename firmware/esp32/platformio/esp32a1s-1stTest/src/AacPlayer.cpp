#include "AacPlayer.h"
#include "webcredentials.h"

AacPlayer::AacPlayer(const char *path, const char *mime_type)
    : dec(&i2s, new AACDecoderHelix()),copier(dec, url), url(WIFI_SSID, WIFI_PASSWORD), path(path), mime_type(mime_type), i2s()
{
}

AacPlayer::~AacPlayer()
{
    url.flush();
    dec.flush();
    copier.end();
    url.end();
    dec.end();
}

void AacPlayer::begin()
{
    auto config = i2s.defaultConfig(TX_MODE);
    i2s.begin(config);
    dec.begin();
    url.begin(path, mime_type);
    i2s.setVolume(0.1);
}

void AacPlayer::play()
{
    copier.copy();
}