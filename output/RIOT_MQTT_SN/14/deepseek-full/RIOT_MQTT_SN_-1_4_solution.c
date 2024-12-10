
#include "mutex.h"
#include "net/emcute.h"
#include "netinet/in.h"
#include "net/ipv6/addr.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "arpa/inet.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "sys/socket.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"
#define MESSAGE_LEN (sizeof(MESSAGE) - 1)
#define SLEEP_TIME (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .port = BROKER_PORT };

int main(void)
{
    // Initialize the topic structure
    topic.name = TOPIC_NAME;
    topic.id = 0;

    // Set up the gateway endpoint
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid broker IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return 1;
    }

    // Main loop to publish messages every 5 seconds
    while (1) {
        // Publish the message
        if (emcute_pub(&topic, MESSAGE, MESSAGE_LEN, 0) != EMCUTE_OK) {
            puts("Error: could not publish message");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_USEC, SLEEP_TIME);
    }

    return 0;
}
