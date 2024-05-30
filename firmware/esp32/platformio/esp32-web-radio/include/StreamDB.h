#pragma once

#include <ArduinoJson.h>

class StreamDB {
public:
    StreamDB(): _doc(2500) {}
    ~StreamDB() { _doc.clear();}
    bool load(const char *dbFile);
    bool save(const char *dbFile);
    bool safeSave(const char *dbFile);  // Save to a temporary file and then rename
    bool getStream(const String& name, String& url) const;
    bool getFirstStream(String& name) const;
    bool getName(int index, String& name) const;
    bool getVolume(const String& name, uint8_t& volume) const;
    bool setVolume(const String& name, uint8_t volume);
    bool setVolumeCurrentChannel(uint8_t volume);
    bool getCurrentStream(String& name) const;
    void setCurrentStream(const String& name);
    void restoreLastStream();
    int size() { return _streams.size(); }

private:
    bool getCrc(const char* dbFile, uint32_t& crc);
    uint32_t getCrc();

    DynamicJsonDocument _doc;
    JsonArray _streams;
    String _lastStream="";
};