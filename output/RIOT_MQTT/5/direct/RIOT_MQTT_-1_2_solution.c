#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/gnrc/netif.h"
#include "xtimer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define PUB_INTERVAL (5U * US_PER_SEC)  // Publish interval in microseconds

static emcute_sock_t sock;
static msg_t queue[8];

static int mqtt_connect(void) {
    sock.remote.family = AF_INET6;
    sock.remote.netif = SOCK_NO_NETIF;

    ipv6_addr_from_str((ipv6_addr_t *)&sock.remote.addr.ipv6, BROKER_IP);
    sock.remote.port = BROKER_PORT;

    if (emcute_con(&sock, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", BROKER_IP, BROKER_PORT);
        return 1;
    }

    printf("Connected to MQTT broker [%s]:%d\n", BROKER_IP, BROKER_PORT);
    return 0;
}

static void mqtt_pub(const char *msg) {
    emcute_pub_t pub_msg;
    pub_msg.topic.name = MQTT_TOPIC;
    pub_msg.data = (void *)msg;
    pub_msg.data_len = strlen(msg);
    pub_msg.flags = EMCUTE_QOS_0;
    
    if (emcute_pub(&sock, &pub_msg) != EMCUTE_OK) {
        printf("Error: unable to publish message\n");
    }
    else {
        printf("Published message: %s\n", msg);
    }
}

int main(void) {
    char *state_msgs[2] = {"work", "not work"};
    size_t index = 0;

    msg_init_queue(queue, sizeof(queue)/sizeof(msg_t));

    if (mqtt_connect() != 0) {
        return 1;
    }

    while (1) {
        mqtt_pub(state_msgs[index]);
        index = (index + 1) % 2;
        xtimer_usleep(PUB_INTERVAL);
    }
    return 0;
}
