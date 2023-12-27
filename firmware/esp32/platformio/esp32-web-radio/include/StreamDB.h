#pragma once

#include <ArduinoJson.h>

class StreamDB {
public:
    StreamDB(): _doc(2500) {}
    ~StreamDB() { _doc.clear();}
    bool open(const char *dbFile);
    bool save(const char *dbFile);
    bool getStream(const char* name, String& url);
    bool getName(int index, String& name);
    bool getVolume(const char* name, uint8_t& volume);
    bool setVolume(const char* name, uint8_t volume);
    int size() { return _streams.size(); }
private:
    DynamicJsonDocument _doc;
    JsonArray _streams;
};