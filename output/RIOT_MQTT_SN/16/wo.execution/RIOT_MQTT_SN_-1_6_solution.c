#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "msg.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "xtimer.h"

#define EMCUTE_PORT         1883
#define EMCUTE_ID_LEN       64U
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_NAME          "device/state"
#define PUB_INTERVAL        (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_topic_t topic;
static uint8_t num_topics = 0;

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) != 1) {
        puts("ERROR: unable to parse gateway address");
        return NULL;
    }

    /* Connect to MQTT-SN gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("ERROR: unable to connect to gateway");
        return NULL;
    }

    /* Get topic ID */
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("ERROR: unable to register topic");
        return NULL;
    }

    while (1) {
        const char *message = "work"; /* Send state 'work' every interval */
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("ERROR: unable to publish data");
        }
        xtimer_usleep(PUB_INTERVAL);
    }

    /* Disconnect after publishing */
    emcute_discon();

    return NULL;
}

int main(void) {
    puts("MQTT-SN publisher application");

    /* Initialize the MQTT-SN thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /* Start the scheduler */
    thread_yield();

    return 0;
}