/*
    RemoteMonitor.h - Library for remote monitoring of ESP32.
    Released into the public domain.

 * Husarnet:
 *  Test connection from linux PC (on which Husarnet daemon is running): `ping6 esp32-web-radio`
 *  `$ socat - tcp:esp32-web-radio:8001
 * set_led 1
 * ok
 * set_led 0
 * ok
*/

#pragma once

#include <Husarnet.h>
#include <SimpleCLI.h>

extern void taskInternetCLI(void *parameter);
class RemoteMonitor
{
public:
    RemoteMonitor(const char *hostName, const int cliPort = 8001);
    void start(const char *joinCode);
    Command addCommand(const char *name, void (*callback)(cmd *c));

private:
    const char *_hostName;
    HusarnetServer _server;
    SimpleCLI _cli;
    friend void taskInternetCLI(void *parameter);
};