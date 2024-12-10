#include <xtimer.h>
#include <net/emcute.h>
#include <net/af.h>
#include <net/sock/udp.h>  // Ensure this is included as required
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_MSG "work"
#define PUB_DELAY (5U * US_PER_SEC)

static emcute_topic_t _topic;
static sock_udp_ep_t broker_endpoint = { .family = AF_INET,
                                         .port = BROKER_PORT };

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    /* Callback handler for receiving messages, not implemented in minimal pub */
}

int main(void) {
    uint8_t buf[64];

    /* Initialize the Emcute library */
    emcute_run(EMCUTE_PORT, "esp32_client");
    
    /* Convert the IP and store it in broker endpoint */
    broker_endpoint.addr.ipv4[0] = 47;
    broker_endpoint.addr.ipv4[1] = 102;
    broker_endpoint.addr.ipv4[2] = 103;
    broker_endpoint.addr.ipv4[3] = 1;

    /* Connect to broker */
    if (emcute_con(&broker_endpoint, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Failed to connect to broker.\n");
        return 1;
    }
    puts("Successfully connected to broker.\n");

    /* Setup topic */
    _topic.name = "state/esp32";
    if (emcute_reg(&_topic) != EMCUTE_OK) {
        puts("Failed to register topic.\n");
        return 1;
    }

    while (1) {
        /* Publish every 5 seconds */
        xtimer_sleep(5);
        size_t len = strlen(PUB_MSG);
        if (emcute_pub(&_topic, PUB_MSG, len, EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Failed to publish message.");
            return 1;
        }
        printf("Published: %s\n", PUB_MSG);
    }

    return 0;
}