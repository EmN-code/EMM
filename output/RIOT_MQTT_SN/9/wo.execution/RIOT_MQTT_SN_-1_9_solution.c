#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"

#define EMCUTE_PORT         (1883U)
#define EMCUTE_ID           (1U)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_TOPIC_MAXLEN (64U)
#define EMCUTE_BUFSIZE      (1024U)
#define SERVER_IP           "47.102.103.1"
#define INTERVAL_SECONDS    (5U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void publish_state(void) {
    emcute_topic_t t;
    t.name = "device/state";
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return;
    }

    const char* data = "work";
    while(true) {
        if (emcute_pub(&t, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Failed to publish data\n");
        } else {
            printf("Data published: %s\n", data);
        }
        xtimer_sleep(INTERVAL_SECONDS);
    }
}

int main(void) {
    msg_init_queue(queue, 8);

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Connection to gateway failed\n");
        return 1;
    }
    else {
        printf("Connected to gateway\n");
    }

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, publish_state, NULL, "publisher");

    return 0;
}