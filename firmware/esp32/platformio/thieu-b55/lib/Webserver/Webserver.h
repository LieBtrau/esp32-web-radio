// based on https://github.com/DominikN/ESP32-to-ESP32/blob/master/src/ESP32-to-ESP32.ino

#pragma once
#include <ESPAsyncWebServer.h>

extern void taskWifi(void *parameter);

class Webserver
{
public:
    Webserver(const char *hostName, const int port=80);
    ~Webserver();
    void start(const char *joinCode);
private:
    AsyncWebServer _server;
    const char *_hostName;
    friend void taskWifi(void *parameter);
};

