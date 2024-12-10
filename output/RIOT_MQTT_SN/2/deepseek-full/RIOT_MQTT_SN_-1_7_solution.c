
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
#include "net/ipv6/addr.h"
#include "mma8x5x.h"
#include "thread.h"
#include "net/sock/udp.h"
#include "stdatomic.h"
#include "stdio.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"

static sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
static emcute_topic_t topic;

void _on_pub(void *arg, int res) {
    (void)arg;
    if (res != EMCUTE_OK) {
        puts("Error on publish");
    }
}

int main(void) {
    // Convert IP address
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error converting IP address");
        return 1;
    }

    // Connect to MQTT-SN gateway
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN gateway");
        return 1;
    }

    // Register topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Publish message every 5 seconds
    while (1) {
        if (emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0) != EMCUTE_OK) {
            puts("Error publishing message");
        }
        thread_sleep(5 * 1000 * 1000); // Sleep for 5 seconds
    }

    return 0;
}
