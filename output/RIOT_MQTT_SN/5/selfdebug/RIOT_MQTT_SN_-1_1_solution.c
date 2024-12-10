#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include "emcute.h"
#include "net/emcute.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL 5U
#define TOPIC_NAME "device/state"
#define TOPIC_ID 1

static emcute_topic_t topic;

static void connect_to_broker(void) {
    sock_udp_ep_t gateway = {
        .family = AF_INET,
        .port = BROKER_PORT
    };
    ipv4_addr_from_str((ipv4_addr_t *)&gateway.addr.ipv4, BROKER_IP);

    if (emcute_connect(&gateway, true, "riot-client", NULL, NULL, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", BROKER_IP, BROKER_PORT);
        return;
    }
    printf("Connected to MQTT-SN broker at [%s]:%d\n", BROKER_IP, BROKER_PORT);
}

static void publish_state(const char *state) {
    emcute_msg_t msg;
    msg.data = (void *)state;
    msg.len = strlen(state);
    msg.type = EMCUTE_MSG_TYPE_PUBLISH;
    msg.flags = EMCUTE_QOS_0;

    if (emcute_publish(&topic, &msg) != EMCUTE_OK) {
        printf("Error: unable to publish data\n");
    } else {
        printf("Published: %s\n", state);
    }
}

int main(void) {
    connect_to_broker();

    topic.name = TOPIC_NAME;
    topic.id = TOPIC_ID;

    while (1) {
        publish_state("work");
        xtimer_sleep(PUBLISH_INTERVAL);
        publish_state("not work");
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}