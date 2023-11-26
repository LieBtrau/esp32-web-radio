#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "AudioCodecs/CodecAACHelix.h"
#include "AudioLibs/AudioKit.h"
#include "webcredentials.h"

URLStream url(WIFI_SSID, WIFI_PASSWORD); // or replace with ICYStream to get metadata
AudioKitStream i2s;                      // final output of decoded stream
// EncodedAudioStream dec(&i2s, new MP3DecoderHelix()); // Decoding stream for MP3
EncodedAudioStream dec;
StreamCopy copier(dec, url); // copy url to decoder

void button2(bool, int, void *) { LOGI("Button 2 pressed"); }

void setup()
{
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // setup i2s
  auto config = i2s.defaultConfig(TX_MODE);
  i2s.begin(config);

  i2s.addAction(PIN_KEY2, button2);//DIP-switch 1 must be ON

  dec.setDecoder(new AACDecoderHelix());
  dec.setStream(&i2s); // Decoding stream for AAC
  dec.begin();
  i2s.setVolume(0.1);

  // mp3 radio
  // url.begin("http://icecast-qmusicbe-cdp.triple-it.nl/joe.mp3","audio/mp3");  //works with mp3 decoder
  // url.begin("http://vrt.streamabc.net/vrt-mnm-mp3-64-4492886","audio/mpeg");  //works with mp3 decoder
  url.begin("http://streamnavs.net:8089/live", "audio/aacp"); // works with aac decoder
}

void loop()
{
  copier.copy();
  i2s.processActions();
}