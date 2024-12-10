#include "ztimer.h"
#include "xtimer.h"
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdio.h>
#include <unistd.h>
#include "esp_system.h"
#include "esp_wifi.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "mutex.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include "mbox.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "dht_params.h"
#include "kernel_defines.h"
#include "arpa/inet.h"

#define MAIN_QUEUE_SIZE (8)
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static char emcute_stack[THREAD_STACKSIZE_DEFAULT];

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL_SEC 5

static emcute_topic_t topic;
static char *topic_name = "workState";
static const char *msg_work = "work";
static const char *msg_idle = "not work";

void *mqtt_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gateway = {};
    gateway.family = AF_INET;
    gateway.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &gateway.addr.ipv4) != 1) {
        printf("Error: could not convert IP address\n");
        return NULL;
    }

    while (1) {
        int rc = emcute_con(&gateway, true, NULL, NULL, 0, 0);
        if (rc == EMCUTE_OK) {
            printf("Successfully connected to gateway\n");
            break;
        } else {
            printf("Connection to gateway failed, retrying\n");
            ztimer_sleep(ZTIMER_SEC, 1);
        }
    }

    topic.name = topic_name;
    emcute_reg(&topic);

    while (1) {
        const char *msg = (rand() % 2) ? msg_work : msg_idle;
        if (emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0) == EMCUTE_OK) {
            printf("Message '%s' published \n", msg);
        } else {
            printf("Error: unable to publish message '%s'\n", msg);
        }
        ztimer_sleep(ZTIMER_SEC, PUBLISH_INTERVAL_SEC);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}