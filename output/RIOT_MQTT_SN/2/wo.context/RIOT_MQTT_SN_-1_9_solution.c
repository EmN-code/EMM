#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/af.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define EMCUTE_ID           "ESP32"
#define SERVER_ADDR         "47.102.103.1"
#define SERVER_PORT         1883
#define PUB_TOPIC_NAME      "device/state"
#define PUB_INTERVAL        (5U * 1000000U)  /* 5 seconds */

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

/* connect to MQTT-SN broker */
static int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .ipv6 = {0}, .port = SERVER_PORT };

    /* convert broker address */
    if (ipv6_addr_from_str((ipv6_addr_t *)gw.addr.ipv6, SERVER_ADDR) == NULL) {
        puts("ERROR: unable to parse broker address\n");
        return 1;
    }

    /* connect to broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("ERROR: unable to connect to broker\n");
        return 1;
    }
    return 0;
}

/* publish data to the MQTT-SN broker */
static void publish_state(char *state) {
    unsigned flags = EMCUTE_QOS_0;
    if (emcute_pub(&topic, state, strlen(state), flags) != EMCUTE_OK) {
        puts("ERROR: unable to publish data\n");
    }
}

int main(void) {
    /* initialize emcute */
    emcute_topic_t t;
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  emcute_thread, NULL, "emcute");

    /* setup connect parameters */
    strcpy(t.name, PUB_TOPIC_NAME);

    /* connect to broker */
    if (connect_broker() != 0) {
        return 1;
    }

    /* register topic */
    topic.name = PUB_TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("ERROR: unable to register topic\n");
        return 1;
    }

    /* publish periodically */
    while (1) {
        publish_state("work");
        xtimer_usleep(PUB_INTERVAL);
    }
    
    return 0;
}