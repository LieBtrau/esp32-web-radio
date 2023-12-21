#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "Mp3Player.h"
#include "AacPlayer.h"



Mp3Player mp3Player("http://icecast-qmusicbe-cdp.triple-it.nl/joe.mp3", "audio/mp3");
AacPlayer aacPlayer("http://streamnavs.net:8089/live", "audio/aacp");

void button2(bool, int, void *)
{ 
  LOGI("button2");
}

void button3(bool, int, void *)
{
  LOGI("button3");
}

void setup()
{
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // setup i2s
  

  // i2s.addAction(PIN_KEY2, button2);//DIP-switch 1 must be ON
  // i2s.addAction(PIN_KEY3, button3);

  

  //mp3Player.begin();    // must be commented out, otherwise the AAC player will not work
  aacPlayer.begin();

}

void loop()
{
  //mp3Player.play();
  aacPlayer.play();
  //i2s.processActions(); // check for button presses
}