#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "msg.h"
#include "kernel_defines.h"
#include "thread.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "dht.h"
#include "dht_params.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "mutex.h"
#include "time_units.h"
#include "timex.h"
#include "stdatomic.h"
#include "net/gcoap.h"

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       ("gertrud")
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_IP  "47.102.103.1"
#define SLEEP_SEC       (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static int on_data(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    (void)topic;
    printf("Received publication on topic "%s":[%i] %s", topic->name, len, in);
    return EMCUTE_OK;
}

static void *emcute_thread(void *arg)
{
    static emcute_topic_t t;
    static char pub_data[32];
    int res;

    (void)arg;

    /* initialize our subscription buffers */
    memset(queue, 0, sizeof(queue));

    /* initialize our mqtts-socket app with our message queue */
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* start emcute thread */
    emcute_init(stack, sizeof(stack), EMCUTE_PRIO, EMCUTE_PORT, EMCUTE_ID);

    /* setup UDP remote endpoint */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    /* connect to gateway */
    while ((res = emcute_con(&gw, true, NULL, NULL, 0, 0)) != EMCUTE_OK) {
        printf("emcute_con failed with %d. Retrying...\n", res);
        xtimer_sleep(1);
    }
    printf("Connected to gateway at %s\n", MQTT_BROKER_IP);

    /* setup the subscription and callbacks */
    emcute_sub_t subscription = { .cb = on_data, .topic = t, .arg = NULL };
    res = emcute_sub(&subscription, EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Subscription to default topic failed: %d\n", res);
        return NULL;
    }

    /* setup publish data */
    strcpy(pub_data, "work");

    while(1) {
        /* publish data every 5 seconds */
        t.name = "status";
        res = emcute_pub(&t, pub_data, strlen(pub_data), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error publishing data: %d\n", res);
        }
        xtimer_sleep(SLEEP_SEC);
    }
    return NULL;
}

int main(void)
{
    /* allocate publish stack thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, THREAD_CREATE_STACKTEST,
                  emcute_thread, NULL, "emcute");

    return 0;
}