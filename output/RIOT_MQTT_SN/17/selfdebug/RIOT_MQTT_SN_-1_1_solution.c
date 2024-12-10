#include <stdio.h>
#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)
#define TOPIC_NAME "device/status"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t msg_queue[8];
static emcute_topic_t topic;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

void *publisher(void *arg) {
    (void)arg;
    while (1) {
        char *msg = "work";  // Change this to "not work" as needed
        emcute_topic_t t;
        t.name = TOPIC_NAME;
        if (emcute_reg(&t) != EMCUTE_OK) {
            printf("Error: unable to register topic\n");
            return NULL;
        }
        emcute_publish(&t, msg, strlen(msg), EMCUTE_QOS_0);
        xtimer_sleep(5);
    }
}

int main(void) {
    printf("Connecting to MQTT-SN broker at %s:%d\n", BROKER_IP, BROKER_PORT);

    static char iface[6];
    gnrc_netif_t *netif = gnrc_netif_get_by_pid(1);

    if (!netif) {
        puts("Error: no network interface found");
        return 1;
    }

    emcute_sock_udp(emcute_create(EMCUTE_ID_MAX, iface), netif, NULL);
    sock_udp_ep_t gw = {.family = AF_INET6, .port = BROKER_PORT};
    ipv6_addr_from_str(&gw.addr.ipv6, BROKER_IP);

    if (emcute_con(&gw, false, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    puts("Starting the publisher thread...");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher, NULL, "publisher");

    return 0;
}