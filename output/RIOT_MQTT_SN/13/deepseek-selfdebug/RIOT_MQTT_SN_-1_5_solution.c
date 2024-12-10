
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <emcute.h>
#include <net/sock/udp.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define MESSAGE "work"

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
        printf("Published: %s to topic: %s\n", MESSAGE, TOPIC_NAME);
    }
}

int main(void) {
    sock_udp_ep_t broker = SOCK_IPV4_EP_ANY;
    broker.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_IP, &broker.addr.ipv4);

    int res = emcute_con(&broker, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    printf("Connected to MQTT-SN broker at %s:%d\n", BROKER_IP, BROKER_PORT);

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    emcute_discon();
    return 0;
}
