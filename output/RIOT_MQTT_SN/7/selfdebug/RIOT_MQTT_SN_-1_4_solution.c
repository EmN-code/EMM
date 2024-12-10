#include "net/mqttsn.h"
#include "net/sock/udp.h"
#include "net/emcute.h"

#include <stdio.h>
#include <string.h>

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U)

static emcute_topic_t topic;

static int publish_state(const char *state) {
    unsigned flags = EMCUTE_QOS_0;
    if (emcute_pub(&topic, state, strlen(state), flags) != EMCUTE_OK) {
        printf("Error: unable to publish our data\n");
        return 1;
    }
    return 0;
}

int main(void) {
    /* setup the remote MQTT-SN gateway address */
    sock_udp_ep_t gw;
    memset(&gw, 0, sizeof(gw));
    gw.family = AF_INET;
    gw.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_ADDRESS, &gw.addr.ipv4);

    /* initialize the emcute stack */
    emcute_con(&gw, true, NULL, NULL, 0, 0);
    topic.name = "work_status";
    emcute_reg(&topic);

    while (1) {
        publish_state("work");
        xtimer_sleep(PUBLISH_INTERVAL);
        publish_state("not work");
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    emcute_discon();
    return 0;
}