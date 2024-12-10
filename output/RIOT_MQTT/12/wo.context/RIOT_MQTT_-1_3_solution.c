#include <net/sock/udp.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL 5

static Network network;
static MQTTClient client;
static unsigned char sendbuf[100], readbuf[100];
static char* topic = "esp32/status";
static MQTTMessage message;

void publish_state(void) {
    char payload[50];
    snprintf(payload, sizeof(payload), "Device state: work or not work");
    message.qos = QOS0;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);
    MQTTPublish(&client, topic, &message);
}

int main(void) {
    int rc;
    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Network connection failed\n");
        return -1;
    }
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        publish_state();
        xtimer_sleep(MQTT_PUBLISH_INTERVAL);
    }

    return 0;
}