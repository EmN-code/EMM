#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "stddef.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"

#define STACKSIZE (THREAD_STACKSIZE_MAIN + 256)
#define TOPIC_NAME "state"
#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883

static char stack[STACKSIZE];
static msg_t queue[8];

static emcute_topic_t topic;

static void *publisher(void *arg) {
    (void)arg;
    char *message = "work";
    while (1) {
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);

    /* Configure endpoint */
    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = MQTT_BROKER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_ADDR);

    /* Connect to gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return 1;
    }

    /* Setup MQTT-SN topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    /* Create the publisher thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, publisher, NULL, "publisher");

    return 0;
}