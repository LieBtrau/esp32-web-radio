/*
    RemoteMonitor.h - Library for remote monitoring of ESP32.
    Released into the public domain.

 * Husarnet:
 *  Test connection from linux PC (on which Husarnet daemon is running): `ping6 esp32-web-radio`
*/

#pragma once

#include <Husarnet.h>
#include <SimpleCLI.h>

extern void taskInternetCLI(void *parameter);
class RemoteMonitor
{
public:
    RemoteMonitor(const char *hostName);
    void start(const char* joinCode);
    Command addCommand(const char* name, void (* callback)(cmd* c));
private:
    HusarnetServer _server;
    SimpleCLI _cli;
    const int CLI_PORT = 8001;
    const char *_hostName;
    friend void taskInternetCLI(void *parameter);
};