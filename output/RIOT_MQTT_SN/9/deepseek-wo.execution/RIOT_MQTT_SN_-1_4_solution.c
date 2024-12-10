
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <thread.h>
#include <xtimer.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"

static emcute_topic_t topic;

void publish_state(void) {
    const char *state = "work";
    int res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing state: %d\n", res);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .port = MQTT_SN_BROKER_PORT };
    inet_aton(MQTT_SN_BROKER_IP, &gw.addr.ipv4);
    gw.family = AF_INET;

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    emcute_run(MQTT_SN_BROKER_PORT, CLIENT_ID);

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
