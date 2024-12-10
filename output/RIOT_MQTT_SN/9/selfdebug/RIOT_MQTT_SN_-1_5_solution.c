#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "xtimer.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;
static char message[64];

static sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(1883, "mosquitto");
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    int res = inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    if (res != 1) {
        printf("Error: unable to parse destination address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return 1;
    }

    topic.name = "status";
   
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to obtain topic ID\n");
        return 1;
    }
    
    while (1) {
        strcpy(message, "work");
        
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        
        xtimer_sleep(5);
    }

    emcute_discon();

    return 0;
}