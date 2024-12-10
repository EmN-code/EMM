#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "stdio.h"
#include "stdbool.h"
#include "paho_mqtt.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL 5  // Interval in seconds

static MQTTClient client;
static Network network;
static char sendbuf[100];
static char recvbuf[100];

void publish_state(MQTTClient *client, const char *topic, const char *message) {
    MQTTMessage mq_message;
    mq_message.qos = QOS0;
    mq_message.retained = 0;
    mq_message.dup = 0;
    mq_message.payload = (void *)message;
    mq_message.payloadlen = strlen(message);

    if (MQTTPublish(client, topic, &mq_message) != SUCCESS) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published: %s\n", message);
    }
}

int main(void) {
    printf("MQTT example
");

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    NetworkConnect(&network, MQTT_BROKER_ADDR, MQTT_BROKER_PORT);

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32client";

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    const char *topic = "device/state";
    const char *state_working = "work";
    const char *state_not_working = "not work";
    bool working = false;

    while (1) {
        working = !working;
        publish_state(&client, topic, working ? state_working : state_not_working);
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}