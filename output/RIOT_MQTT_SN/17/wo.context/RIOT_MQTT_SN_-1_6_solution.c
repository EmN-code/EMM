#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/sock/udp.h>

#include <net/emcute.h>

#define EMCUTE_ID           "mqtt-client"
#define EMCUTE_PORT         1883
#define SERVER_IP           "47.102.103.1"
#define STATE_TOPIC_NAME    "device/state"
#define STATE_WORK          "work"
#define STATE_NOT_WORK      "not work"
#define SLEEP_DURATION_SEC  5

static emcute_topic_t state_topic;

static sock_udp_ep_t server = {
    .family = AF_INET,
    .port = EMCUTE_PORT
};

int main(void) {
    msg_t queue[8];
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    if (emcute_con(&server, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to the MQTT-SN broker at %s\n", SERVER_IP);
        return 1;
    }

    state_topic.name = STATE_TOPIC_NAME;
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }

    while(1) {
        const char *state = STATE_WORK;
        if (emcute_pub(&state_topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Failed to publish state\n");
        } else {
            printf("State published: %s\n", state);
        }
        sleep(SLEEP_DURATION_SEC);
    }

    return 0;
}