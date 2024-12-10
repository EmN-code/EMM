#include <stdlib.h>
#include <stdio.h>
#include <thread.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <string.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define _IPV4_ADDR_LEN      (4U)
#define BROKER_ADDRESS      ("47.102.103.1")
#define BROKER_PORT         (1883)
#define TOPIC_NAME          ("state")

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;
static char *topic_name = TOPIC_NAME;

void *mqttsn_publisher(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = {.family = AF_INET6, .port = BROKER_PORT};
    uint32_t addr;

    if (sock_ip4_str2addr(BROKER_ADDRESS, &addr) != 0) {
        puts("Error: unable to parse broker address");
        return NULL;
    }

    memcpy(gw.addr.ipv6, &addr, sizeof(addr));

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to MQTT-SN broker\n");
        return NULL;
    }

    topic.name = topic_name;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic\n");
        return NULL;
    }

    while (1) {
        const char *message = "work";
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  mqttsn_publisher, NULL, "mqttsn_thread");

    return 0;
}