#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "dht_params.h"
#include "dht.h"
#include "assert.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define PUBLISH_INTERVAL (5U * 1000U * 1000U)

static char sendbuf[256];
static char readbuf[256];

int main(void) {
    // Initialize MQTT Client and network options
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    // Initialize MQTT client
    MQTTClientInit(&client, &network, PUBLISH_INTERVAL, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    MQTTConnect(&client, &connectData);

    // Publish loop
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = false;
        message.dup = false;
        message.payload = (void *)"work";
        message.payloadlen = strlen("work");

        // Publish the message
        MQTTPublish(&client, MQTT_TOPIC, &message);

        // Wait before next publish
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }
    return 0;
}