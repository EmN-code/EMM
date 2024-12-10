
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <sys/socket.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state_msg[] = "work";

static void publish_state(void) {
    emcute_topic_t topic;
    memset(&topic, 0, sizeof(topic));
    topic.name = TOPIC;

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return;
    }

    res = emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish message\n");
    } else {
        printf("Published: %s\n", state_msg);
    }
}

int main(void) {
    sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;
    gw.port = BROKER_PORT;
    gw.family = AF_INET;
    gw.netif = SOCK_ADDR_ANY_NETIF;

    int res = sock_udp_create(&gw, NULL, BROKER_ADDR, 0);
    if (res < 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    return 0;
}
