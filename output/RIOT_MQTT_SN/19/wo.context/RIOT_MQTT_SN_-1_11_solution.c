#include <string.h>                 // Ensure alphabetical order
#include <xtimer.h>                 // Always good to ensure timers
#include <net/ipv6/addr.h>          // Needed for inet_pton function
#include <stdio.h>
#include <emcute.h>                 // Import emcute

#define SERVER_ADDR     "47.102.103.1"
#define SERVER_PORT     1883
#define PUBLISH_INTERVAL 5000000    // 5 seconds

static emcute_topic_t topic;

void connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    if (inet_pton(AF_INET, SERVER_ADDR, &gw.addr) != 1) {
        printf("Error: unable to parse the gateway address\n");
        return;
    }

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to the gateway [%d]\n", res);
    }
}

void setup_topic(void) {
    topic.name = "device/status";
    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic [%d]\n", res);
    }
}

void publish_state(void) {
    const char *state = "work";
    unsigned flags = EMCUTE_QOS_0;
    int res = emcute_pub(&topic, state, strlen(state), flags);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish [%d]\n", res);
    }
}

int main(void) {
    connect_to_broker();
    setup_topic();

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}