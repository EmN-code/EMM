
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];

static void publish_state(void) {
    emcute_topic_t topic;
    memset(&topic, 0, sizeof(topic));
    topic.name = TOPIC_NAME;

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return;
    }

    const char *message = STATE_MESSAGE;
    res = emcute_pub(&topic, message, strlen(message), 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish message\n");
    } else {
        printf("Published: %s\n", message);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .port = EMCUTE_PORT };
    int res = inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    if (res != 1) {
        printf("Error: unable to convert IP address\n");
        return 1;
    }

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
