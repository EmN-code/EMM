#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <time_units.h>
#include "kernel_defines.h"

#define EMCUTE_ID          "riot"
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS          (16U)
#define TOPIC_MAXLEN       (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_q[NUMOFSUBS];
static emcute_sub_t subscriptions[NUMOFSUBS];

static char *topics[NUMOFSUBS];
static emcute_topic_t t;
static sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };

void *pub_thread(void *arg) {
    (void)arg;
    while(1) {
        char message[] = "state: work";
        int res = emcute_pub(&t, message, sizeof(message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Pub error: %d\n", res);
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    // Start the emcute thread
    msg_init_queue(msg_q, NUMOFSUBS);
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, EMCUTE_ID);

    // Setup the MQTT-SN client
    while(1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Connection successful!");
            break;
        }
        else {
            printf("Failed to connect: %d\n", res);
        }
    }

    strcpy(t.name, "topic1");
    while(1) {
        if (emcute_reg(&t) == EMCUTE_OK) {
            break;
        } else {
            printf("Error registering topic\n");
        }
    }

    // Create the publication thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, pub_thread, NULL, "pub_thread");

    return 0;
}