#include "esp_wifi.h"
#include "stdlib.h"
#include "stdio.h"
#include "stddef.h"
#include "msg.h"
#include "stdbool.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/ipv6/addr.h"
#include "net/gcoap.h"
#include "dht.h"
#include "dht_params.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "mutex.h"
#include "paho_mqtt.h"
#include "stdatomic.h"
#include "string.h"
#include "time_units.h"
#include "timex.h"
#include "xtimer.h"
#include "ztimer.h"
#include "assert.h"
#include "net/emcute.h"
#include "thread.h"

#define EMCUTE_ID           "mqtt-sn"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT         (1883U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_topic_t _topic;
static char _status[10];

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, 8);
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    /* start emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /* connect to gateway */
    sock_udp_ep_t gw = {.family = AF_INET,
                        .port = EMCUTE_PORT};
    inet_pton(gw.family, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to gateway\n");
        return 1;
    }

    /* setup topic */
    _topic.name = "status";
    if (emcute_reg(&_topic) != EMCUTE_OK) {
        printf("error: unable to register topic\n");
        return 1;
    }

    /* publish data every five seconds */
    while (1) {
        snprintf(_status, sizeof(_status), "work");
        if (emcute_pub(&_topic, _status, strlen(_status), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}