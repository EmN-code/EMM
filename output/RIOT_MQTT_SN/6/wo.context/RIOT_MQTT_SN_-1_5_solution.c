#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/inet/ntop.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define EMCUTE_PORT CONFIG_EMCUTE_DEFAULT_PORT
#define EMCUTE_ID "esp32_r"

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .port = EMCUTE_PORT };

static const char *state = "work";

int main(void) {
    /* Set up Gateway Address */
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    /* Connect to MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return -1;
    }

    /* Register the topic */
    topic.name = "device/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return -1;
    }

    while (1) {
        /* Publish the state every 5 seconds */
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}