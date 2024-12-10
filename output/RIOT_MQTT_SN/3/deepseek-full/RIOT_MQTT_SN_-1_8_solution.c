
#include "dht_params.h"
#include "stdbool.h"
#include "stddef.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "mutex.h"
#include "net/emcute.h"
#include "dht.h"
#include "kernel_defines.h"
#include "mma8x5x.h"
#include "net/sock/udp.h"
#include "stdatomic.h"
#include "stdio.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"
#define STATE_MESSAGE_LEN (sizeof(STATE_MESSAGE) - 1)
#define PUBLISH_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
static emcute_topic_t topic;

int main(void)
{
    // Initialize EMCUTE
    emcute_init();

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        emcute_discon();
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        if (emcute_pub(&topic, STATE_MESSAGE, STATE_MESSAGE_LEN, 0) != EMCUTE_OK) {
            puts("Error: could not publish message");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Disconnect from the gateway
    emcute_discon();

    return 0;
}
