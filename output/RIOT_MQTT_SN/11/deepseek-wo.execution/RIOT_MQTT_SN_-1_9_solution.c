
#include <net/emcute.h>
#include <xtimer.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw;

void publish_state(void) {
    static int state = 0;
    const char *msg = (state++ % 2) ? MSG_NOT_WORK : MSG_WORK;
    emcute_pub(&topic, msg, strlen(msg), 0);
}

int main(void) {
    // Initialize the MQTT-SN gateway endpoint
    gw.family = AF_INET;
    gw.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Could not connect to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: Could not register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
