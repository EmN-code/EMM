
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define EMCUTE_BUFSIZE         (128U)
#define EMCUTE_TOPIC_MAXLEN    (32U)
#define EMCUTE_PORT            (1883U)
#define EMCUTE_SERVER_IP       "47.102.103.1"
#define EMCUTE_CLIENT_ID       "esp32_client"
#define EMCUTE_TOPIC_NAME      "state"
#define EMCUTE_STATE_MSG       "work"
#define EMCUTE_PUBLISH_INTERVAL (5U)

static char emcute_buf[EMCUTE_BUFSIZE];

static void _publish_state(void)
{
    emcute_topic_t topic;
    topic.name = EMCUTE_TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the gateway

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return;
    }

    res = emcute_pub(&topic, EMCUTE_STATE_MSG, strlen(EMCUTE_STATE_MSG), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Published state: %s\n", EMCUTE_STATE_MSG);
    }
}

int main(void)
{
    sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;
    gw.port = EMCUTE_PORT;

    if (inet_pton(AF_INET, EMCUTE_SERVER_IP, &gw.addr.ipv4) <= 0) {
        printf("Error parsing server IP address\n");
        return 1;
    }

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    printf("Connected to MQTT-SN broker at %s:%d\n", EMCUTE_SERVER_IP, EMCUTE_PORT);

    while (1) {
        _publish_state();
        xtimer_sleep(EMCUTE_PUBLISH_INTERVAL);
    }

    return 0;
}
