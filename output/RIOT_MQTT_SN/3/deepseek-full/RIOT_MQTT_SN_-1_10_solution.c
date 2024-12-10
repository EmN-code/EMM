
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
#define TOPIC "state"
#define STATE_MSG "work or not work"
#define STATE_MSG_LEN (sizeof(STATE_MSG) - 1)

static emcute_topic_t topic;
static sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };

int main(void)
{
    int result;

    // Initialize the EMCUTE library
    emcute_init();

    // Convert the IP address string to a byte-represented IP address
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error: Invalid IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    result = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (result != EMCUTE_OK) {
        puts("Error: Could not connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC;
    result = emcute_reg(&topic);
    if (result != EMCUTE_OK) {
        puts("Error: Could not register topic");
        emcute_discon();
        return 1;
    }

    // Publish the state message every five seconds
    while (1) {
        result = emcute_pub(&topic, STATE_MSG, STATE_MSG_LEN, 0);
        if (result != EMCUTE_OK) {
            puts("Error: Could not publish message");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    // Disconnect from the MQTT-SN broker
    emcute_discon();

    return 0;
}
