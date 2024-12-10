#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_mqtt_client"
#define PUBLISH_TOPIC "device/state"
#define PUBLISH_INTERVAL (5 * US_PER_SEC)  // publish every 5 seconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[80], readbuf[80];

void publish_state(void) {
    MQTTMessage message;
    char payload[20];
    sprintf(payload, "work");
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);
    MQTTPublish(&client, PUBLISH_TOPIC, &message);
    printf("Published: %s\n", payload);
}

int main(void) {
    int rc;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Network connect failed: %d\n", rc);
        return -1;
    }
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;
    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("MQTT connect failed: %d\n", rc);
        return -1;
    }
    printf("MQTT connected\n");

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return 0;
}