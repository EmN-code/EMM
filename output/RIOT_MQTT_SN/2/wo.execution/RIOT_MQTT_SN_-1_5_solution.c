#include "xtimer.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"
#include "net/ipv6/addr.h"
#include "arpa/inet.h"

#define EMCUTE_PORT     (1885U)
#define EMCUTE_ID       "emcute-client"
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)

#define MQTT_SN_PORT    (1883U)
#define SERVER_ADDR     "47.102.103.1"
#define TOPIC_NAME      "state"
#define BUF_SIZE        (128U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_PORT };

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;    /* should never be reached */
}

static int mqttsn_pub(char *message) {
    unsigned flags = EMCUTE_QOS_2;
    return emcute_pub(&topic, message, strlen(message), flags);
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* start emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /* parse gateway address */
    if (inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4) != 1) {
        printf("error: unable to parse gateway address\n");
        return 1;
    }

    /* connect to MQTT-SN gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to gateway\n");
        return 1;
    }

    /* setup subscription topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("error: unable to obtain topic ID\n");
        return 1;
    }

    /* publish state every 5 seconds */
    while (1) {
        mqttsn_pub("work");
        xtimer_sleep(5);
        mqttsn_pub("not work");
        xtimer_sleep(5);
    }

    return 0;
}