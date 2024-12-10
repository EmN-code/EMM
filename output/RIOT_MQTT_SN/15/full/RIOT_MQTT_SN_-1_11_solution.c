#include "msg.h"
#include "mutex.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "ztimer.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "stdbool.h"
#include "stdlib.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdio.h"
#include "mbox.h"
#include "esp_system.h"
#include "stddef.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "xtimer.h"
#include "unistd.h"
#include "esp_wifi.h"
#include "net/gcoap.h"
#include "timex.h"

#define EMCUTE_ID     ("gertrud")
#define EMCUTE_PRIO  (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME  "state"

static emcute_topic_t topic;

void *mqtt_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, 8);

    /* initialize our subscription topics */
    memset(&topic, 0, sizeof(topic));
    topic.name = TOPIC_NAME;

    /* start emcute thread */
    emcute_run(SERVER_PORT, EMCUTE_ID);

    /* connect to broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

    /* Convert IP address to proper format */
    if (inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse IP" );
        return NULL;
    }

    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Connected to %s\n", SERVER_ADDR);
            break;
        } else {
            printf("Connection failed. Retrying in 1 second...\n");
            xtimer_sleep(1);
        }
    }

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Failed to register topic");
        return NULL;
    }

    while (1) {
        const char *state = "work or not work";
        unsigned flags = EMCUTE_QOS_0;

        if (emcute_pub(&topic, state, strlen(state), flags) != EMCUTE_OK) {
            puts("Error: unable to publish data");
        } else {
            printf("Published %s\n", state);
        }
        
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  mqtt_thread, NULL, "mqtt_thread");

    return 0;
}