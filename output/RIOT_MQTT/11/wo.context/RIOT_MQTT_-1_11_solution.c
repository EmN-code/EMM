#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "esp32/status"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void publish_status(MQTTClient *client, const char *status) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)status;
    message.payloadlen = strlen(status);
    MQTTPublish(client, MQTT_TOPIC, &message);
}

int main(void) {
    printf("Starting MQTT client\n");
    NetworkInit(&network);
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != SUCCESS) {
        printf("Failed to connect to network\n");
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "riot_esp32";

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    while (1) {
        publish_status(&client, "work");
        xtimer_sleep(PUBLISH_INTERVAL);
        publish_status(&client, "not work");
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    NetworkDisconnect(&network);
    return 0;
}