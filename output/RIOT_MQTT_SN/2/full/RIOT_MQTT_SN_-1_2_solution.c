#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "net/gcoap.h"
#include "net/emcute.h"

#define EMCUTE_PORT (1883U)
#define SERVER_ADDR "47.102.103.1"
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "device/state"

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };

void on_message(const emcute_topic_t *topic, void *data, size_t len) {
    (void) topic;
    (void) data;
    (void) len;
    /* Handle incoming message */
}

int main(void) {
    /* Initialize the gateway address */
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "47.102.103.1");

    /* Start the emcute thread */
    emcute_run(EMCUTE_PORT, CLIENT_ID);

    /* Connect to the MQTT-SN broker */
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("Failed to connect to the broker");
        return 1;
    }

    /* Setup the MQTT-SN topic */
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        puts("Failed to register topic");
        return 1;
    }

    /* Main loop to publish the state periodically */
    while (1) {
        const char *state = "work";
        res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Publish error: %d\n", res);
        }
        xtimer_sleep(5);
    }

    return 0;
}