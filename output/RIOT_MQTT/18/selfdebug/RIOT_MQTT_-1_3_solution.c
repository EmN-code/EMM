#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define STATE_TOPIC "device/status"
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds

static char sendbuf[128];
static char readbuf[128];

int main(void) {
    /* Initialize networking */
    Network network;
    NetworkInit(&network);
    
    /* Initialize MQTT client */
    MQTTClient client;
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    /* Establish network connection */
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != SUCCESS) {
        printf("Failed to connect to network.\n");
        return 1;
    }
    printf("Connected to MQTT broker.\n");

    /* Set MQTT connection data */
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32Client";

    /* Connect to MQTT broker */
    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker.\n");
        return 1;
    }
    printf("Connected to MQTT broker successfully.\n");

    /* Loop to publish state every 5 seconds */
    while (1) {
        MQTTMessage message;
        message.qos = QOS1;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void *)"work or not work";
        message.payloadlen = strlen("work or not work");

        if (MQTTPublish(&client, STATE_TOPIC, &message) != SUCCESS) {
            printf("Failed to publish message.\n");
        }
        else {
            printf("Message published successfully.\n");
        }

        xtimer_sleep(PUBLISH_INTERVAL / 1000000); // Convert microseconds to seconds
    }

    return 0;
}