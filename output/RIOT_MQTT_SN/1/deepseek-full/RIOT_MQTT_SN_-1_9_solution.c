
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <kernel_defines.h>
#include <dht.h>
#include <net/ipv6/addr.h>
#include <mma8x5x.h>
#include <stdatomic.h>
#include <mutex.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define BUF_SIZE 64

static sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
static emcute_topic_t topic;
static char buf[BUF_SIZE];

void publish_state(void) {
    static bool state = false;
    snprintf(buf, sizeof(buf), "%s", state ? "work" : "not work");
    state = !state;
    emcute_pub(&topic, buf, strlen(buf), 0);
}

int main(void) {
    // Convert IP address string to byte representation
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    // Connect to the MQTT-SN gateway
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Could not connect to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: Could not register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
