
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <ztimer.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"

static sock_udp_t sock;
static sock_udp_ep_t remote;
static emcute_topic_t topic;

void connect_to_broker(void) {
    remote.family = AF_INET;
    remote.port = htons(EMQX_SERVER_PORT);
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return;
    }
}

void publish_state(void) {
    int res = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish message: %d\n", res);
    }
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
