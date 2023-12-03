#pragma once

#include <ArduinoJson.h>

class StreamDB {
public:
    StreamDB(): _doc(2000) {}
    ~StreamDB() { _doc.clear();}
    bool open(const char *dbFile);
    bool getStream(const int index, String& name, String& url);
    int size() { return _streams.size(); }
private:
    DynamicJsonDocument _doc;
    JsonArray _streams;
};