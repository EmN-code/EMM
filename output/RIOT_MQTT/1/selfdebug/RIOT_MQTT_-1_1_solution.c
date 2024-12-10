#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "sys/shell.h"
#include "stdio.h"
#include "stdlib.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "device/state"
#define MQTT_PUBLISH_INTERVAL (5U * 1000U * 1000U) // 5 seconds

static MQTTClient client;
static Network network;

void mqtt_publish_state(void) {
    MQTTMessage message;
    const char *state = "work";  // state to be published, either "work" or "not work"

    message.qos = QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    if (MQTTPublish(&client, MQTT_TOPIC, &message) != SUCCESS) {
        printf("Failed to publish MQTT message\n");
    }
    else {
        printf("Published state: %s\n", state);
    }
}

int main(void) {
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != SUCCESS) {
        printf("Failed to connect to network\n");
        return -1;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = MQTT_CLIENT_ID;

    if (MQTTConnect(&client, &data) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }
    else {
        printf("Connected to MQTT broker\n");
    }

    while (1) {
        mqtt_publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000); // sleep for 5 seconds
    }

    return 0;
}