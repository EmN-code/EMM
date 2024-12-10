
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define MQTT_SN_BROKER_ADDR "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };

void publish_state(void)
{
    const char *state = "work";
    int res;

    res = sock_udp_create(&gw, NULL, &gw, 0);
    if (res < 0) {
        puts("Error creating UDP socket");
        return;
    }

    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        puts("Error registering topic");
        return;
    }

    res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        puts("Error publishing state");
        return;
    }

    puts("State published successfully");
}

int main(void)
{
    int res;

    /* Initialize the topic */
    topic.name = TOPIC_NAME;
    topic.id = 0;

    /* Set the gateway address */
    inet_pton(AF_INET, MQTT_SN_BROKER_ADDR, &gw.addr.ipv4);

    /* Connect to the MQTT-SN broker */
    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
