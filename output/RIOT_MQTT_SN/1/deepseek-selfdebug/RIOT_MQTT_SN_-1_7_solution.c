
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"

static emcute_topic_t topic;

void publish_state(void) {
    char buf[64];
    strncpy(buf, STATE_MESSAGE, sizeof(buf));
    int res = emcute_pub(&topic, buf, strlen(buf), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Published: %s\n", buf);
    }
}

int main(void) {
    sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;
    gw.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        printf("Error converting IP address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker\n");
        return 1;
    }

    printf("Connected to MQTT-SN broker\n");

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error registering topic\n");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
