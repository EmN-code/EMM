#include <ztimer.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>

#define EMCUTE_PORT     (1883U)
#define SERVER_ADDR     "47.102.103.1"
#define PUBLISH_INTERVAL (5U)

static emcute_topic_t state_topic;
static char topic_name[] = "device/state";

int main(void) {
    printf("Starting MQTT-SN example with emcute\n");

    /* Initialize the MQTT-SN topic */
    state_topic.name = topic_name;

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv6); // Use ipv6 for storing address

    /* Connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", SERVER_ADDR, EMCUTE_PORT);
        return 1;
    }
    printf("Connected to the broker at [%s]:%d\n", SERVER_ADDR, EMCUTE_PORT);

    /* Publish the state every 5 seconds */
    while (1) {
        const char *state_msg = "work"; // Assume the device is in "work" state

        if (emcute_pub(&state_topic, state_msg, strlen(state_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        } else {
            printf("Published state: %s\n", state_msg);
        }

        ztimer_sleep(ZTIMER_SEC, PUBLISH_INTERVAL);
    }

    /* Disconnect (although this will not be reached here) */
    emcute_discon();
    return 0;
}
