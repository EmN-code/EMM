#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define BROKER_PORT    1883
#define BROKER_IP      "47.102.103.1"
#define EMCUTE_ID      "esp32"
#define PUB_INTERVAL   (5U * US_PER_SEC)

static emcute_topic_t topic;
static char *status_pub_topic = "device/status";
static char *status_msg = "work";

/* Function to connect to MQTT-SN broker */
static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    gw.family = AF_INET6;
    inet_pton(AF_INET6, BROKER_IP, &gw.addr.ipv6);

    while (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%d\n", BROKER_IP, BROKER_PORT);
        xtimer_sleep(5);
    }

    return EMCUTE_OK;
}

/* Function to publish data */
static void publish_status(void) {
    topic.name = status_pub_topic;
    topic.id = 0; /* Topic ID will be set by the register function */

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("error: unable to register topic\n");
        return;
    }

    while (1) {
        if (emcute_pub(&topic, status_msg, strlen(status_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        } else {
            printf("Data published: %s\n", status_msg);
        }
        xtimer_usleep(PUB_INTERVAL);
    }
}

int main(void) {
    printf("MQTT-SN Example for RIOT\n");
    puts("Connecting to MQTT-SN broker\n");

    if (connect_to_broker() == EMCUTE_OK) {
        puts("Connected successfully\n");
        publish_status();
    }

    return 0;
}