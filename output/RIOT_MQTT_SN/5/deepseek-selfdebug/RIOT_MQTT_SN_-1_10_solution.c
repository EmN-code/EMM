
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <emcute.h>

#define EMCUTE_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work"

static char emcute_stack[EMCUTE_STACKSIZE];

static void publish_state(void) {
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the gateway

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: could not register topic (%d)\n", res);
        return;
    }

    res = emcute_pub(&topic, MESSAGE, strlen(MESSAGE), 0);
    if (res != EMCUTE_OK) {
        printf("Error: could not publish message (%d)\n", res);
    } else {
        printf("Published: %s\n", MESSAGE);
    }
}

int main(void) {
    sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);
    gw.port = BROKER_PORT;

    printf("Connecting to MQTT-SN broker at %s:%d\n", BROKER_IP, BROKER_PORT);
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: could not connect to MQTT-SN broker (%d)\n", res);
        return 1;
    }

    printf("Connected to MQTT-SN broker\n");

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
