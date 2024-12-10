#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>  // Required for the sock-related structs and functions
#include <arpa/inet.h>     // Required for inet_pton function
#include "net/emcute.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;

static void on_connected(void) {
    // Logic for handling post connection can be added here
}

static void on_work_state_publish(void) {
    const char *state_message = "work";
    emcute_pub(&topic, state_message, strlen(state_message), EMCUTE_QOS_0);
    printf("Published state: %s\n", state_message);
}

int main(void) {
    // Initialize UDP endpoint for broker
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    // Initialize emcute
    emcute_run(BROKER_PORT, "mqtt-sn-client");
    emcute_con(&gw, true, NULL, NULL, 0, 0);

    // Register topic
    topic.name = "device/state";
    emcute_reg(&topic);

    // Schedule cyclic publishing of work state
    while (1) {
        on_work_state_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}