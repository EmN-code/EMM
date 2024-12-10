#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/emcute.h"
#include <string.h>  // for memset
#include <stdio.h>  // for printf
#include "MQTTClient.h"

#define EMCUTE_PORT     (1883U)
#define MQTT_BROKER_IP  "47.102.103.1"
#define TOPIC_NAME      "device/state"
#define PUBLISH_INTERVAL (5U)

emcute_topic_t topic;
emcute_sock_t sock;

static const char *state_msg[] = {"work", "not work"};

void on_connected(void) {
    sock_udp_ep_t gw = {.family=AF_INET, .port=EMCUTE_PORT};
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return;
    }

    topic.name = TOPIC_NAME;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        emcute_discon();
        return;
    }

    printf("Connected to broker %s:%d\n", MQTT_BROKER_IP, EMCUTE_PORT);
}

void publish_state(void) {
    static size_t msg_idx = 0;
    size_t len = strlen(state_msg[msg_idx]);

    if (emcute_pub(&topic, state_msg[msg_idx], len, EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Error: unable to publish\n");
    } else {
        printf("Published: %s\n", state_msg[msg_idx]);
    }

    msg_idx = (msg_idx + 1) % 2;
}

int main(void) {
    puts("MQTT-SN connection example\n");

    on_connected();

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}