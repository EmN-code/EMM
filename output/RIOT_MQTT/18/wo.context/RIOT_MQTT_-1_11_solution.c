#include <xtimer.h>  // Include the xtimer library for timer functions
#include <string.h>
#include <stdio.h>
#include "MQTTClient.h"  // Include the MQTTClient library last

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds
#define PUBLISH_TOPIC "device/state"
#define DEVICE_STATE "work"


static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)DEVICE_STATE;
    message.payloadlen = strlen(DEVICE_STATE);
    
    int ret = MQTTPublish(&client, PUBLISH_TOPIC, &message);
    if (ret != SUCCESS) {
        printf("Failed to publish message: %d\n", ret);
    }
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to network: %d\n", rc);
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;    
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_ESP32";

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return -1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        publish_state();
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
    }

    return 0;
}
