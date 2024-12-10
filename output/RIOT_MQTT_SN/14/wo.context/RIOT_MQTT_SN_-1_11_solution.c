#include <string.h>  // for string operations
#include <stdio.h>  // for printf functions
#include <stdlib.h>  // for atoi function
#include <net/ipv4/addr.h>  // for configuring IPv4 addresses
#include <xtimer.h>  // for timing functions
#include "net/emcute.h" // for emcute library functionality

#define NUMOFSUBS (16U)
#define TOPIC_MAXLEN (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[NUMOFSUBS];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr);

    // Set EMCUTE connection parameters
    unsigned flags = EMCUTE_QOS_0;  // Define QoS level 0
    int res;

    // Connect to MQTT-SN broker
    if ((res = emcute_con(&gw, true, NULL, NULL, 0, 0)) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway [%d]\n", res);
        return 1;
    }

    emcute_topic_t t;
    t.name = "state";

    char *work_state[] = {"work", "not work"};

    // Publish message every 5 seconds
    for (;;) {
        int random_state = rand() % 2;  // simulate a state
        res = emcute_pub(&t, work_state[random_state], strlen(work_state[random_state]), flags);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data [%d]\n", res);
        }

        xtimer_sleep(5);
    }

    return 0;
}