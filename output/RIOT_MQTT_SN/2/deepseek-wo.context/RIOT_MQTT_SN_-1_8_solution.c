
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_esp32"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char state_msg[] = "work";

static void publish_state(void)
{
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0;  // Topic ID will be assigned by the broker

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic (%d)\n", res);
        return;
    }

    res = emcute_pub(&topic, state_msg, strlen(state_msg), 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish message (%d)\n", res);
    } else {
        printf("Published: %s\n", state_msg);
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .port = EMCUTE_PORT };
    int res = inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    if (res <= 0) {
        printf("Error: invalid address\n");
        return 1;
    }

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker (%d)\n", res);
        return 1;
    }

    printf("Connected to MQTT-SN broker\n");

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
