// /**
//  * @file headphone.ino
//  * @author Phil Schatzmann
//  * @brief Headphone Detection Demo
//  * @date 2021-12-10
//  * 
//  * @copyright Copyright (c) 2021
//  * 
//  */
// #include "Arduino.h"
// #include "AudioKitHAL.h"
// #include "../examples/output/SineWaveGenerator.h"

// AudioKit kit;
// SineWaveGenerator wave;
// const int BUFFER_SIZE = 1024;
// uint8_t buffer[BUFFER_SIZE];

// void setup() {
//   LOGLEVEL_AUDIOKIT = AudioKitInfo; 
//   Serial.begin(115200);
//   // open in write mode
//   auto cfg = kit.defaultConfig(AudioOutput);
//   kit.begin(cfg);

//   // 1000 hz
//   wave.setFrequency(1000);
//   wave.setSampleRate(cfg.sampleRate());

// }

// void loop() {
//   size_t l = wave.read(buffer, BUFFER_SIZE);
//   kit.write(buffer, l);
// }

// /**
//  * @file streams-generator_sinfromtable-audiokit.ino
//  * @brief Tesing SineFromTable with output on audiokit
//  * @author Phil Schatzmann
//  * @copyright GPLv3
//  */
 
// #include "AudioTools.h"
// #include "AudioLibs/AudioKit.h"

// uint16_t sample_rate=32000;
// uint8_t channels = 2;                                      // The stream will have 2 channels 
// SineFromTable<int16_t> sineWave(32000);                // subclass of SoundGenerator with max amplitude of 32000
// GeneratedSoundStream<int16_t> sound(sineWave);             // Stream generated from sine wave
// AudioKitStream out; 
// //CsvStream<int16_t> out(Serial);
// int sound_len=1024;
// StreamCopy copier(out, sound, sound_len);                             // copies sound into i2s
// int freq = 122;

// // Arduino Setup
// void setup(void) {  
//   // Open Serial 
//   Serial.begin(115200);
//   AudioLogger::instance().begin(Serial, AudioLogger::Warning);

//   // start I2S
//   Serial.println("starting I2S...");
//   auto config = out.defaultConfig(); //TX_MODE
//   config.sample_rate = sample_rate; 
//   config.channels = channels;
//   config.bits_per_sample = 16;
//   out.begin(config);

//   // Setup sine wave
//   auto cfg = sineWave.defaultConfig();
//   cfg.channels = channels;
//   cfg.sample_rate = sample_rate;
//   sineWave.begin(cfg, freq);
//   Serial.println("started...");
// }

// // Arduino loop - copy sound to out 
// void loop() {
//   // the length is defined by sound_len
//   copier.copy();
//   // increase frequency
//   freq += 10;
//   if (freq>=10000){
//     freq = 20;
//   }
//   sineWave.setFrequency(freq);
// }
// /**
//  * @file streams-url_mp3-audiokit.ino
//  * @author Phil Schatzmann
//  * @brief decode MP3 stream from url and output it on I2S on audiokit
//  * @version 0.1
//  * @date 2021-96-25
//  * 
//  * @copyright Copyright (c) 2021
//  */

// // install https://github.com/pschatzmann/arduino-libhelix.git

#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "AudioLibs/AudioKit.h"


URLStream url("3WO8kiazq8Ok17E","8y43859N30A72qe");  // or replace with ICYStream to get metadata
AudioKitStream i2s; // final output of decoded stream
EncodedAudioStream dec(&i2s, new MP3DecoderHelix()); // Decoding stream
StreamCopy copier(dec, url); // copy url to decoder


void setup(){
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);  

  // setup i2s
  auto config = i2s.defaultConfig(TX_MODE);
  i2s.begin(config);

  // setup I2S based on sampling rate provided by decoder
  dec.begin();

  // mp3 radio
  url.begin("https://22673.live.streamtheworld.com/RADIO1_128.mp3","audio/mp3");

}

void loop(){
  copier.copy();
}