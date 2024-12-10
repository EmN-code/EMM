#include "thread.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "arpa/inet.h"
#include "dht_params.h"
#include "mqttclient.h"
#include "mutex.h"
#include "mma8x5x_regs.h"

#define EMCUTE_PORT         1883
#define EMCUTE_ID           (sizeof(emcute_topic_t))
#define BROKER_IP           "47.102.103.1"
#define MQTT_TOPIC          "state"
#define SLEEP_INTERVAL      (5U)
#define PUB_MSG             "work"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_sub_t subscriptions[1];
static emcute_topic_t topic;

static void *pub_thread(void *arg) {
    (void)arg;
    while (1) {
        int res = emcute_pub(&topic, PUB_MSG, sizeof(PUB_MSG), EMCUTE_QOS_0);
        if (res == EMCUTE_OK) {
            printf("Published message: %s\n", PUB_MSG);
        } else {
            printf("Failed to publish message\n");
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    /* initialize emcute */
    emcute_init(queue, sizeof(queue) / sizeof(queue[0]), MQTT_TOPIC);

    /* set topic name */
    topic.name = MQTT_TOPIC;

    /* connect to broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Could not connect to broker\n");
        return 1;
    }

    /* get topic ID */
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }

    /* start publisher thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, pub_thread, NULL, "pub thread");

    return 0;
}