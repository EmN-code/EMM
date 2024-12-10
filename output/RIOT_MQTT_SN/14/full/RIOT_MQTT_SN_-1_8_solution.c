#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <msg.h>
#include <ztimer.h>

#define EMCUTE_PORT        (1883U)
#define SERVER_ADDRESS     "47.102.103.1"
#define PUB_INTERVAL       (5U)
#define PUB_TOPIC_NAME     "device/state"
#define PUB_STATE          "work"
#define PUB_STATE_LENGTH   (4U)

static emcute_topic_t topic;
static sock_udp_ep_t gw = {
    .family = AF_INET,
    .port = EMCUTE_PORT
};

static bool connect_to_broker(void) {
    char *gw_address = SERVER_ADDRESS;
    if (inet_pton(AF_INET, gw_address, &gw.addr.ipv4) != 1) {
        printf("Error: Unable to parse IPv4 address\n");
        return false;
    }

    int attempt = 0;
    while (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        if (++attempt > 5) {
            printf("Error: Unable to connect to broker\n");
            return false;
        }
        printf("Retrying connection to broker...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return true;
}

static void publish_state(void) {
    printf("Attempting to publish state\n");

    while (true) {
        int attempt = 0;
        while (emcute_pub(&topic, PUB_STATE, PUB_STATE_LENGTH, 0) != EMCUTE_OK) {
            if (++attempt > 5) {
                printf("Error: Unable to publish\n");
                break;
            }
            printf("Retrying publish...\n");
            if (!connect_to_broker()) {
                return;
            }
            ztimer_sleep(ZTIMER_MSEC, 500);
        }
        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL * 1000);
    }
}

static int setup_mqtt(void) {
    strcpy(topic.name, PUB_TOPIC_NAME);
    int attempt = 0;
    while (emcute_reg(&topic) != EMCUTE_OK) {
        if (++attempt > 5) {
            printf("Error: Unable to register topic\n");
            return -1;
        }
        printf("Retrying topic registration...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    if (!connect_to_broker()) {
        return 1;
    }
    
    if (setup_mqtt() < 0) {
        return 1;
    }

    publish_state();

    return 0;
}