
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <arpa/inet.h>  // Include this for inet_pton

#define EMCUTE_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"

static char emcute_stack[EMCUTE_STACKSIZE];

static void publish_state(void) {
    emcute_topic_t topic;
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    topic.id = 0;  // Topic ID will be assigned by the gateway

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return;
    }

    res = emcute_pub(&topic, STATE_MESSAGE, strlen(STATE_MESSAGE), 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish message\n");
    } else {
        printf("Published: %s\n", STATE_MESSAGE);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    printf("Connecting to MQTT-SN broker at %s:%d\n", BROKER_IP, BROKER_PORT);
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
