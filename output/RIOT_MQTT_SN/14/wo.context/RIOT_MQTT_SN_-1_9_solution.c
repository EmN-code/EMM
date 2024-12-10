#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>

// MQTT-SN broker address and port
#define BROKER_ADDR      "47.102.103.1"
#define BROKER_PORT      1883
#define STATE_TOPIC_NAME "device/state"
#define STATE_WORK       "work"
#define STATE_NOT_WORK   "not work"

#define EMCUTE_PORT      (BROKER_PORT)
#define EMCUTE_ID        "esp32"

static emcute_topic_t topic;

int main(void) {
    printf("RIOT MQTT-SN Example\n");

    // Initialize the emcute network
    emcute_topic_t t;
    memset(&t, 0, sizeof(t));

    strcpy(t.name, STATE_TOPIC_NAME);
    int res = emcute_reg(&t);

    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic '%s'\n", STATE_TOPIC_NAME);
        return 1;
    }

    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = EMCUTE_PORT
    };

    inet_pton(AF_INET, BROKER_ADDR, &gw.addr.ipv4);

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);

    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return 1;
    }

    // Publish work state every five seconds
    while (1) {
        res = emcute_pub(&t, STATE_WORK, strlen(STATE_WORK), EMCUTE_QOS_0);

        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data, %d\n", res);
        } else {
            printf("Published: %s\n", STATE_WORK);
        }

        xtimer_sleep(5);
    }

    return 0;
}