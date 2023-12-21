#include "Mp3Player.h"
#include "webcredentials.h"

Mp3Player::Mp3Player(const char *path, const char *mime_type)
    : dec(&i2s, new MP3DecoderHelix()), copier(dec, url), url(WIFI_SSID, WIFI_PASSWORD), path(path), mime_type(mime_type), i2s()
{
}

Mp3Player::~Mp3Player()
{
    url.flush();
    dec.flush();
    copier.end();
    url.end();
    dec.end();
}

void Mp3Player::begin()
{
    auto config = i2s.defaultConfig(TX_MODE);
    i2s.begin(config);
    dec.begin();
    url.begin(path, mime_type);
    i2s.setVolume(0.1);
}

void Mp3Player::play()
{
    copier.copy();
}