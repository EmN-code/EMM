
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define MESSAGE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void publish_state(void) {
    emcute_topic_t topic;
    memset(&topic, 0, sizeof(topic));
    topic.name = TOPIC_NAME;

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return;
    }

    res = emcute_pub(&topic, MESSAGE, strlen(MESSAGE), 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish message\n");
    } else {
        printf("Published: %s\n", MESSAGE);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    emcute_run(SERVER_PORT, CLIENT_ID);

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
