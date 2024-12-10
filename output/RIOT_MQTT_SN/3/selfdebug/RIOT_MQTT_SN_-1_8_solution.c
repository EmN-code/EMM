#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <net/conn/ip.h>

#define EMCUTE_ID           "esp32"
#define EMCUTE_PORT         1883
#define EMCUTE_TOPIC_STATE  "state"
#define SERVER_IP           "47.102.103.1"
#define STATE_WORK          "work"
#define STATE_NOT_WORK      "not work"

static emcute_topic_t state_topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

/* Function to connect to MQTT-SN broker */
static int connect_to_broker(void) {
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    state_topic.name = EMCUTE_TOPIC_STATE;
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    return 0;
}

/* Function to publish state */
static int publish_state(const char *state) {
    if (emcute_pub(&state_topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
        return 1;
    }
    return 0;
}

int main(void) {
    /* Setup server address */
    ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, SERVER_IP);

    /* Connect to the MQTT-SN broker */
    if (connect_to_broker() != 0) {
        return 1;
    }

    /* Start publishing loop */
    while (1) {
        publish_state(STATE_WORK);
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}