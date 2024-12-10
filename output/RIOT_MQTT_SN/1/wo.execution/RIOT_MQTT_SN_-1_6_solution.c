#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "msg.h"
#include "mutex.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "time_units.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMBER_OF_TOPICS    (1U)
#define MAIN_QUEUE_SIZE     (8)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[MAIN_QUEUE_SIZE];

static emcute_topic_t topic;
static char *topics[NUMBER_OF_TOPICS];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(CONFIG_NETIF_DEFAULT_MTU, "mqtt-client");
    return NULL;    // This should never be reached
}

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Received publication for topic %d [%.*s]\n",
           topic->id, len, (char *)data);
}

int main(void) {
    msg_init_queue(queue, MAIN_QUEUE_SIZE);
    memset(topics, 0, (NUMBER_OF_TOPICS * sizeof(char *)));

    /* Start emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO,
                  THREAD_CREATE_STACKTEST, emcute_thread,
                  NULL, "emcute");

    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    /* connect to broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }
    printf("Successfully connected to broker at 47.102.103.1:1883\n");
    
    /* set the topic we want to publish to */
    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID");
        return 1;
    }

    char *states[2] = {"work", "not work"};
    uint8_t state_index = 0;

    while (1) {
        /* toggle state */
        state_index = !state_index;
        
        /* Publish the message */
        if (emcute_pub(&topic, states[state_index], strlen(states[state_index]), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        } else {
            printf("Published: %s\n", states[state_index]);
        }

        /* sleep for 5 seconds */
        xtimer_sleep(5);
    }

    /* This should never be reached */
    return 0;
}