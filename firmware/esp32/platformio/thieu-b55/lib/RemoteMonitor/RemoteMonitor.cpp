#include "RemoteMonitor.h"

void taskInternetCLI(void *parameter);
static RemoteMonitor *pRemoteMonitor = NULL;

RemoteMonitor::RemoteMonitor(const char *hostName) : _server(CLI_PORT), _hostName(hostName)
{
    pRemoteMonitor = this;
}

void RemoteMonitor::start(const char *joinCode)
{
    //Husarnet.selfHostedSetup("default");
    Husarnet.join(joinCode, _hostName);
    Husarnet.start();
    TaskHandle_t xHandle = NULL;
    xTaskCreate(
        taskInternetCLI, /* Task function. */
        "taskHusarnet",  /* String with name of task. */
        20000,           /* Stack size in bytes. */
        NULL,            /* Parameter passed as input of the task */
        2,               /* Priority of the task. */
        &xHandle);       /* Task handle. */
    configASSERT(xHandle);
}

Command RemoteMonitor::addCommand(const char *name, void (*callback)(cmd *c))
{
    return _cli.addCmd(name, callback);
}

void taskInternetCLI(void *parameter)
{
    pRemoteMonitor->_server.begin(8001);

    while (1)
    {
        HusarnetClient client = pRemoteMonitor->_server.available();
        if (client)
        {
            Serial.println("New Client.");
            String currentLine = "";
            Serial.printf("connected: %d\r\n", client.connected());
            while (client.connected())
            {
                if (client.available())
                {
                    char c = client.read();
                    currentLine += c;

                    if (c == '\n')
                    {
                        pRemoteMonitor->_cli.parse(currentLine);
                        currentLine = "";
                        client.println("ok");
                    }
                }
            }
            client.stop();
            Serial.println("Client disconnected.");
            Serial.println("");
        }
        else
        {
            delay(200);
        }
    }
}