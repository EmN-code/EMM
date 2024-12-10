#include <ztimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/emcute.h>
#include <msg.h>

#define EMCUTE_PORT     (1883U)
#define SERVER_ADDR     "47.102.103.1"
#define TOPIC_NAME      "device/state"
#define PUB_MSG         "work"
#define PUB_INTERVAL    (5U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

void *publisher(void *arg) {
    (void) arg; /* to silence unused warnings */

    xtimer_sleep(1); /* allow time for network setup */

    while (1) {
        if (emcute_reg(&topic) != EMCUTE_OK) {
            printf("[Error] Unable to register topic
");
        }
        else {
            printf("Publishing message...
");
            if (emcute_pub(&topic, PUB_MSG, strlen(PUB_MSG), EMCUTE_QOS_0) != EMCUTE_OK) {
                printf("[Error] Unable to publish
");
            }
        }
        xtimer_sleep(PUB_INTERVAL);
    }
    
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);
    
    /* Initialize thread for the publisher */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher, NULL, "publisher");

    printf("Starting mqtt-sn example
");

    /* Connect to MQTT-SN gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[Error] Unable to connect to gateway
");
        return 1;
    }

    /* copy topic name into topic structure */
    topic.name = TOPIC_NAME;

    /* execute should never reach here with blocking calls */
    return 0;
}