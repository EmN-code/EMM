#include "xtimer.h"
#include "ztimer.h"
#include "net/emcute.h"
#include "msg.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "arpa/inet.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "unistd.h"

#define EMCUTE_PORT     1883
#define TOPIC_NAME      "work_status"
#define BROKER_IP       "47.102.103.1"
#define DELAY           (5U * US_PER_SEC)

static emcute_topic_t topic;

static void *publish_state(void *arg) {
    (void)arg;
    char *data = "work";

    while(1) {
        printf("Publishing data: %s\n", data);
        if (emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return 1;
    }

    strcpy(topic.name, TOPIC_NAME);
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    publish_state(NULL);

    return 0;
}