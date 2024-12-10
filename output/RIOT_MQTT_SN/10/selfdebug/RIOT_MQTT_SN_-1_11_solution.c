#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "thread.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "emcute.h"

#define MQTT_BROKER_ADDR    "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define PUB_TOPIC           "state/status"
#define PUB_INTERVAL        (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_topic_t topic;
static char status[12];

static void *event_loop(void *arg) {
    (void)arg;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        snprintf(status, sizeof(status), "work"); // Or "not work"
        int res = emcute_pub(&topic, status, strlen(status), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error publishing message: %d\n", res);
        }

        xtimer_periodic_wakeup(&last_wakeup, PUB_INTERVAL * US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    /* start the event loop thread */
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  event_loop, NULL, "event_loop");

    /* initialize emcute */
    if (emcute_init() != EMCUTE_OK) {
        puts("Error initializing emcute");
        return 1;
    }

    /* setup target address */
    sock_udp_ep_t gw = { .family = AF_INET6, .netif = 6 }; 
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_ADDR);
    gw.port = MQTT_BROKER_PORT;

    /* connect to MQTT broker */
    if (emcute_con(&gw, false, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Could not connect to gateway");
        return 1;
    }

    /* set up publish topic */
    topic.name = PUB_TOPIC;
    emcute_reg(&topic);

    return 0;
}