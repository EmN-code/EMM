#include <xtimer.h>
#include <string.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <arpa/inet.h>
#include <msg.h>
#include <mutex.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <esp_wifi.h>
#include <dht.h>
#include <dht_params.h>
#include <timex.h>
#include <ztimer.h>
#include <time_units.h>
#include <net/gcoap.h>

#define EMCUTE_ID "riot-client"
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS (16U)
#define TOPIC_MAXLEN (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

/* MQTT-SN connection and topic configuration */
static emcute_topic_t topic;
static char *message = "work";
static sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(1883, EMCUTE_ID);
    return NULL; /* unreachable */
}

int main(void) {
    int res;
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* Start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* convert address */
    res = inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    if (res == 0) {
        puts("inet_pton: malformed address");
        return 1;
    }
    else if (res < 0) {
        puts("inet_pton: error");
        return 1;
    }

    /* connect to MQTT-SN gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }

    /* setup subscribe topic */
    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }

    while (1) {
        /* publish state message */
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}