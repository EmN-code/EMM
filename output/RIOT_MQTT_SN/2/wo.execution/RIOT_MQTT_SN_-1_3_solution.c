#include "string.h"
#include "mma8x5x.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "thread.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "net/emcute.h"
#include "stdio.h"
#include "net/sock/udp.h"
#include "stdlib.h"
#include "dht_params.h"

#define EMCUTE_ID "device_id"
#define EMCUTE_PORT 1883
#define SERVER_ADDR "47.102.103.1"
#define TOPIC_NAME "device/state"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_topic_t topic;

void *emcute_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    gw.netif = SOCK_ADDR_ANY_NETIF;
    inet_pton(AF_INET6, SERVER_ADDR, &gw.addr.ipv6);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        const char *data = "work";  // or "not work" based on some condition
        if (emcute_pub(&topic, data, strlen(data), 0) != EMCUTE_OK) {
            puts("Error: unable to publish data");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return 0;
}