#include <emcute.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_TOPIC "device/state"
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static emcute_topic_t _client_topic;

static void _publish_state(const char *state) {
    emcute_msg_t msg;
    msg.data = (void *)state;
    msg.len = strlen(state);
    msg.type = EMCUTE_MSG_TYPE_DATA;
    msg.flags = EMCUTE_MSG_FLAG_NONE;
    if (emcute_pub(&_client_topic, &msg) != EMCUTE_OK) {
        printf("error: unable to publish to topic '%s'\n", _client_topic.name);
    }
}

int main(void) {
    // Setup the connection to the broker
    if (emcute_con(BROKER_IP, BROKER_PORT, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    // Publish loop
    while (1) {
        _client_topic.name = CLIENT_TOPIC;
        _client_topic.id = 0; // Typically, 0 indicates no pre-assigned topic ID

        printf("Publishing state: WORK\n");
        _publish_state("WORK");
        xtimer_sleep(5);

        printf("Publishing state: NOT WORK\n");
        _publish_state("NOT WORK");
        xtimer_sleep(5);
    }

    return 0;
}