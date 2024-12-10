#include "thread.h"
#include "stdio.h"
#include "stdlib.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "net/gcoap.h"
#include "event/thread.h"

#define EMCUTE_PRIO       (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS         (1U)
#define TOPIC_MAXLEN      (64U)
#define MQTT_BROKER_IP    "47.102.103.1"
#define MQTT_BROKER_PORT  (1883U)
#define PUB_INTERVAL      (5U)

static char mqtt_stack[THREAD_STACKSIZE_MAIN];
static msg_t mqtt_queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topic_pub[TOPIC_MAXLEN] = "status";

static void *mqtt_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = {.family = AF_INET, .port = MQTT_BROKER_PORT};
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway.\n");
        return NULL;
    }

    emcute_topic_t t;
    memset(&t, 0, sizeof(t));
    t.name = topic_pub;
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("Error: unable to register topic.\n");
        emcute_discon();
        return NULL;
    }

    while (1) {
        char *status_msg = "work";
        if (emcute_pub(&t, status_msg, strlen(status_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data.\n");
        }

        xtimer_sleep(PUB_INTERVAL);
    }

    emcute_discon();
    return NULL;
}

int main(void) {
    puts("MQTT-SN Publisher");

    msg_init_queue(mqtt_queue, ARRAY_SIZE(mqtt_queue));

    thread_create(mqtt_stack, sizeof(mqtt_stack), EMCUTE_PRIO, 0,
                  mqtt_thread, NULL, "mqtt_thread");

    return 0;
}