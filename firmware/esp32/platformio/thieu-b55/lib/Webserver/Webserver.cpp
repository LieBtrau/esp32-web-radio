/**
 * @file Webserver.cpp
 * @author Christoph (you@domain.com)
 * @brief Simple webserver for ESP32, with RESTful API.
 *  Usage : `curl -X GET esp32-web-radio`
 * Usage : in web browser: `http://esp32-web-radio
 * @version 0.1
 * @date 2023-12-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "Webserver.h"
#include <Husarnet.h>

void taskWifi(void *parameter);
static Webserver* pWebserver = NULL;

Webserver::Webserver(const char *hostName, const int port) : _server(port), _hostName(hostName)
{
    pWebserver = this;
}

Webserver::~Webserver()
{
}

void Webserver::start(const char *joinCode)
{
    Husarnet.selfHostedSetup("default");
    Husarnet.join(joinCode, _hostName);

    _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send(200, "application/json", String("{\"status\" : \"200\"}")); });

    TaskHandle_t xHandle = NULL;
    xTaskCreate(
        taskWifi,   /* Task function. */
        "taskWifi", /* String with name of task. */
        10000,      /* Stack size in bytes. */
        NULL,       /* Parameter passed as input of the task */
        2,          /* Priority of the task. */
        &xHandle);  /* Task handle. */
    configASSERT(xHandle);
}

void taskWifi(void *parameter)
{
    // Start Husarnet VPN Client
    Husarnet.start();

    // Start HTTP server
    pWebserver->_server.begin();

    while (1)
    {
        // while (WiFi.status() == WL_CONNECTED)
        // {
        //     delay(500);
        // }
        // LOG("WiFi disconnected, reconnecting\r\n");
        delay(500);
        // stat = wifiMulti.run();
        // LOG("WiFi status: %d\r\n", (int)stat);
    }
}