#include <stdio.h>
#include <string.h>
#include <emcute.h>
#include <msg.h>
#include <xtimer.h>

#define INTERVAL (5U * 1000000U)  // Publish interval of 5 seconds
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

#define TOPIC_NAME "device/state"

static emcute_topic_t device_topic;
static char message[64];

static void publish_state(const char *state) {
    strncpy(message, state, sizeof(message));
    message[sizeof(message) - 1] = '\0';

    int res = emcute_pub(&device_topic, message, strlen(message), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish data (error %d)\n", res);
    }
}

int main(void) {
    msg_init_queue(message_queue, sizeof(message_queue) / sizeof(msg_t));

    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    device_topic.name = TOPIC_NAME;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker\n");
        return 1;
    }

    emcute_reg(&device_topic);

    while (1) {
        publish_state("work");
        xtimer_usleep(INTERVAL);
        publish_state("not work");
        xtimer_usleep(INTERVAL);
    }

    return 0;
}