#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "time_units.h"
#include "timex.h"
#include "net/sock/util.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "dht_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_system.h"

#define EMCUTE_ID       "device123"
#define EMCUTE_PORT     1883
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)

#define MQTT_TOPIC_NAME "state_topic"
#define MQTT_BROKER_IP  "47.102.103.1"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void *emcute_thread(void *arg) {
    static emcute_topic_t topic;
    topic.name = MQTT_TOPIC_NAME;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

    printf("Converting IP address...\n");
    int res = inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);
    if (res != 1) {
        puts("Error: cannot convert IP address");
        return NULL;
    }

    while (1) {
        int rc = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (rc != EMCUTE_OK) {
            printf("Failed to connect to gateway, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        printf("Connected to gateway\n");
        break;
    }

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        const char *message = "work";
        printf("Publishing message: %s\n", message);
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, THREAD_CREATE_STACKTEST,
                  emcute_thread, NULL, "emcute_thread");

    return 0;
}
