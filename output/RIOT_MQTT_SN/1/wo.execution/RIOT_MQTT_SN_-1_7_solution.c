#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdio.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "assert.h"

#define EMCUTE_ID           "esp32_client"
#define EMCUTE_PORT         1883
#define EMCUTE_TOPIC_MAXLEN 64
#define SERVER_ADDR         "47.102.103.1"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_topic_t topic;

static void *publisher(void *arg) {
    (void)arg;
    char *message = "work";
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);
    
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%i\n", SERVER_ADDR, EMCUTE_PORT);
        return NULL;
    }

    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return NULL;
    }

    while (1) {
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        } else {
            printf("Published message: %s\n", message);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publisher, NULL, "publisher");

    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return 0;
}