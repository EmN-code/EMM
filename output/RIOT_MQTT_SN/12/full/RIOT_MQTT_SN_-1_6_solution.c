#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "net/emcute.h"
#include "msg.h"

#define EMCUTE_PORT     (1883U)
#define MQTT_BROKER_IP  "47.102.103.1"
#define NUMOFSUBS       (16U)
#define TOPIC_MAXLEN    (64U)
#define PUB_INTERVAL    (5U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];
static char mqtt_client_id[17];

static int mqtt_connect(void) {
    sock_udp_ep_t gw = {.family = AF_INET, .port = EMCUTE_PORT};
    int res = inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);
    if(res != 1) {
        printf("[Error] Unable to parse IP address\n");
        return -1;
    }

    while(1) {
        res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("[Info] Connected to gateway at %s\n", MQTT_BROKER_IP);
            break;
        }
        else {
            printf("[Error] Connection failed, retrying...\n");
            xtimer_sleep(1);
        }
    }
    return 0;
}

static int publish_state(const char *state) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    strncpy(t.name, "state", sizeof(t.name));
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("[Error] Unable to register topic\n");
        return -1;
    }
    
    if (emcute_pub(&t, state, strlen(state), flags) != EMCUTE_OK) {
        printf("[Error] Unable to publish data\n");
        return -1;
    }

    printf("[Info] Published `%s` on topic `%s`\n", state, t.name);
    return 0;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    if (mqtt_connect() != 0) {
        printf("[Error] MQTT connection failed\n");
        return -1;
    }

    while(1) {
        publish_state("work");
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}