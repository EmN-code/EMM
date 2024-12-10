#include "stdio.h"
#include "stdlib.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"

#define EMCUTE_ID           "riot"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define STACK_SIZE          (THREAD_STACKSIZE_MAIN)
#define SERVER_ADDR         "47.102.103.1"
#define SERVER_PORT         1883
#define TOPIC_NAME          "state"
#define PUB_INTERVAL        (5U)

static char stack[STACK_SIZE];
static emcute_topic_t topic;

int main(void) {
    sock_udp_ep_t remote;

    /* convert IP string to actual IP address format */
    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) != 1) {
        puts("[Error] Malformed IP address");
        return 1;
    }
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    /* connect to MQTT-SN broker */
    int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("[Error] Unable to connect to the broker");
        return 1;
    }

    /* set up the topic we want to publish to */
    topic.name = TOPIC_NAME;
    topic.id = 0;  // assume topic is pre-registered and use id 0

    while (1) {
        const char *state = "work";
        uint8_t qos = 0; // Use QoS level 0
        int result = emcute_pub(&topic, state, strlen(state), qos);

        if (result != EMCUTE_OK) {
            puts("[Error] Publication failed");
        } else {
            puts("Publication success");
        }
        /* wait for next publication */
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}