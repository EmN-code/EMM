#include <stdio.h>
#include "MQTTPacket.h"
#include "MQTTClient.h"
#include "emcute.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define PUBLISH_INTERVAL 5
#define TOPIC_NAME "device/state"

static emcute_topic_t topic;
static MQTTClient client;

void publish_state(MQTTClient *client) {
    static int state = 0;
    const char *msg = state ? "work" : "not work";
    state = !state;

    MQTTMessage message;
    message.payload = (void *)msg;
    message.payloadlen = strlen(msg);
    message.qos = QOS0;
    message.retained = 0;

    MQTTPublish(client, TOPIC_NAME, &message);
    printf("Published state: %s\n", msg);
}

int main(void) {
    Network n;
    MQTTClientInit(&client, &n, PUBLISH_INTERVAL * 1000, NULL, 0, NULL, 0);

    // Connect to MQTT broker
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    MQTTConnect(&client, &connectData, EMQX_SERVER_IP, EMQX_SERVER_PORT);
    printf("Connected to MQTT-SN broker at %s:%d\n", EMQX_SERVER_IP, EMQX_SERVER_PORT);

    // Prepare the topic
    topic.name = TOPIC_NAME;
    topic.id = 0;

    // Main loop to publish state
    while (1) {
        publish_state(&client);
        sleep(PUBLISH_INTERVAL);
    }

    return 0;
}