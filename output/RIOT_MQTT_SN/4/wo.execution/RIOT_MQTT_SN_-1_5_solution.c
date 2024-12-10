#include <ztimer.h>
#include <xtimer.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdint.h>
#include <msg.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <dht.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define EMCUTE_PORT         (1885U)
#define EMCUTE_ID           "esp32_node"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define PUB_INTERVAL_SEC    (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static sock_udp_ep_t gw = {.family = AF_INET6, .port = EMCUTE_PORT};
static const char *topic_name = "device/status";
static const char *pub_message = "work";

void *pub_thread(void *arg) {
    emcute_topic_t t;
    t.name = topic_name;

    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("[ERROR] unable to obtain topic ID\n");
        return NULL;
    }

    while (1) {
        if (emcute_pub(&t, pub_message, strlen(pub_message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("[ERROR] unable to publish data\n");
        }
        ztimer_sleep(ZTIMER_SEC, PUB_INTERVAL_SEC);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    int res = inet_pton(AF_INET6, "47.102.103.1", &gw.addr.ipv6);
    if (res <= 0) {
        printf("[ERROR] unable to parse IP address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[ERROR] unable to connect to gateway\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, pub_thread, NULL, "pub_thread");

    return 0;
}