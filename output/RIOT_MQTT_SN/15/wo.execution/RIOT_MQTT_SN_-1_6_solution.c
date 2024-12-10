#include <stdio.h>
#include <stdbool.h>
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "xtimer.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define SEND_PERIOD         (5000000U)
#define BROKER_PORT         (1883U)
#define BROKER_IP           "47.102.103.1"

static msg_t _msg_q[8];
static char _stack[THREAD_STACKSIZE_MAIN];

static emcute_sub_t _subscriptions[1];
static char _topic_pub[] = "status";

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    printf("Received publication for topic '%s' [%i]: %.*s\n",
           topic->name, (int)topic->id, (int)len, in);
}

int main(void)
{
    msg_init_queue(_msg_q, sizeof(_msg_q) / sizeof(_msg_q[0]));
    memset(_subscriptions, 0, sizeof(_subscriptions));

    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    inet_pton(AF_INET6, BROKER_IP, &gw.addr.ipv6);

    while (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Failed to connect, retrying in 5 seconds...");
        xtimer_sleep(5);
    }

    puts("Connected to MQTT-SN broker");
    
    emcute_topic_t t;
    memset(&t, 0, sizeof(t));
    t.name = _topic_pub;

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Failed to register topic");
        return 1;
    }

    printf("Registered topic '%s' with id: %i\n",
           t.name, t.id);

    while (true) {
        char* pub_data = "work";
        unsigned flags = EMCUTE_QOS_0; 

        if (emcute_pub(&t, pub_data, strlen(pub_data), flags) != EMCUTE_OK) {
            printf("Failed to publish data '%s'\n", pub_data);
        } else {
            printf("Published data '%s'\n", pub_data);
        }

        xtimer_usleep(SEND_PERIOD);
    }

    return 0;
}