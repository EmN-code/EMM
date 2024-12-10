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
#include "paho_mqtt.h"
#include "MQTTClient.h"

// Define the IP and port of the MQTT broker
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

static MQTTClient client;
static Network network;

static unsigned char sendbuf[100], readbuf[100];

void mqtt_message_publish(void) 
{
    static const char *topic = "device/state";
    static MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)"work";
    message.payloadlen = strlen((char*)message.payload);
    int rc = MQTTPublish(&client, topic, &message);
    if (rc != SUCCESS) {
        // Handle error
    }
}

int main(void) 
{
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != SUCCESS) {
        // Handle connection error
        return rc;
    }
    
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "esp32_client";

    rc = MQTTConnect(&client, &options);
    if (rc != SUCCESS) {
        // Handle MQTT connection failure
        return rc;
    }

    ztimer_clock_t *clock = ZTIMER_MSEC;
    ztimer_t timer;

    // Configure timer to call the publish function every 5000 ms (5 seconds)
    ztimer_set(clock, &timer, 5000);
    while(1) {
        mqtt_message_publish();
        ztimer_sleep(clock, 5000);
    }
    
    return 0;
}