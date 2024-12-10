
#include <stdlib.h>
#include <net/gcoap.h>
#include <dht.h>
#include <net/sock/udp.h>
#include <kernel_defines.h>
#include <mma8x5x.h>
#include <net/ipv6/addr.h>
#include <stdio.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <dht_params.h>
#include <thread.h>
#include <mutex.h>
#include <mma8x5x_regs.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"

static emcute_sub_t sub;
static emcute_topic_t topic;

void mqtt_cb(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Received message on topic %s: %.*s\n", topic->name, (int)len, (char *)data);
}

int main(void) {
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid server address");
        return 1;
    }

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    sub.topic = topic;
    sub.cb = (emcute_cb_t)mqtt_cb;
    if (emcute_sub(&sub, 0) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return 1;
    }

    while (1) {
        const char *msg = (rand() % 2 == 0) ? MSG_WORK : MSG_NOT_WORK;
        if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        xtimer_sleep(5);
    }

    return 0;
}
