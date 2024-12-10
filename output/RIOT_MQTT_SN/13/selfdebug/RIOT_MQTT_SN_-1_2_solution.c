#include <stdio.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)
#define MQTT_TOPIC "device/state"

static emcute_topic_t _topic;

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

    /* convert IP address string to network address */
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return 1;
    }

    /* connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to the server\n");
        return 1;
    }

    /* register the topic */
    _topic.name = MQTT_TOPIC;
    if (emcute_reg(&_topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    /* publish messages every 5 seconds */
    while (1) {
        const char *msg = "work";
        if (emcute_pub(&_topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish\n");
        }
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}