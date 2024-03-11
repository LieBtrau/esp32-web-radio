
#include "Webserver.h"
#include "RemoteMonitor.h"
#include "husarnet_id.h"

static const char *hostName = "esp32-web-radio";
static RemoteMonitor remoteMonitor(hostName);
static Command set_led;
static Webserver server(hostName);
static void set_led_callback(cmd *c);

void set_led_callback(cmd *c)
{
    Command cmd(c);

    String state = cmd.getArg("state").getValue();
    ESP_LOGI(, "set_led_callback: %s", state.c_str());
}

void init_remoteControl()
{
    remoteMonitor.start(HUSARNET_JOINCODE);
    set_led = remoteMonitor.addCommand("set_led", set_led_callback);
    set_led.addPosArg("state");
}