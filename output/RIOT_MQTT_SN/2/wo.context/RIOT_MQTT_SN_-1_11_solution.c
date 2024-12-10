#include <string.h>  // Included for string operations
#include <stdio.h>    // Standard IO library
#include <arpa/inet.h> // For inet_pton
#include <xtimer.h>  // For timer
#include <net/emcute.h>

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;
static msg_t msg_queue[8];

#define EMCUTE_PORT     1883U
#define SERVER_ADDR     "47.102.103.1"
#define TOPIC_NAME      "state"

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);

    sock_udp_ep_t gw;
    gw.family = AF_INET;
    gw.port = EMCUTE_PORT;
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);

    emcute_run(EMCUTE_PORT, "esp32_node");

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Could not connect to broker
");
        return 1;
    }

    topic.name = TOPIC_NAME;
    emcute_reg(&topic);

    while (1) {
        const char *state = "work";
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data
");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    if (emcute_discon() != EMCUTE_OK) {
        puts("Error: unable to disconnect from broker
");
        return 1;
    }
    return 0;
}