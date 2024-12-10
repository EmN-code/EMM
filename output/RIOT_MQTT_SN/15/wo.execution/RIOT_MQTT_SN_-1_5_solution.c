#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stdbool.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "stdio.h"
#include "sys/socket.h"
#include "unistd.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "MQTTClient.h"
#include "thread.h"
#include "time_units.h"

#define EMCUTE_PORT         (1883U)
#define SERVER_IP           "47.102.103.1"
#define PUBLISH_INTERVAL    (5000U) /* 5 seconds */
#define TOPIC_NAME          "state"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, 8);

    /* setup MQTT-SN environment */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    /* connect to MQTT-SN broker */
    if (emcute_con(&gw, true, TOPIC_NAME, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", SERVER_IP, EMCUTE_PORT);
        return NULL;
    }

    /* prepare the topic */
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic "%s"\n", TOPIC_NAME);
        return NULL;
    }

    /* main loop */
    while (1) {
        const char *message = (esp_system_get_free_heap_size() % 2) == 0 ? "work" : "not work";
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        xtimer_sleep(PUBLISH_INTERVAL / 1000);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  emcute_thread, NULL, "emcute");

    /* keep the main thread busy */
    while (1) {
        thread_yield();
    }
    return 0;
}