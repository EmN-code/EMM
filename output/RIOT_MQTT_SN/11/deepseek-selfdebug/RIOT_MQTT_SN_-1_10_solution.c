
#include <stdio.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define MQTT_SN_TOPIC "state"
#define MQTT_SN_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char state_msg[] = "work";

static void publish_state(void) {
    emcute_topic_t topic;
    topic.name = MQTT_SN_TOPIC;
    topic.id = 0;  // Topic ID will be assigned by the gateway

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: could not register topic (%d)\n", res);
        return;
    }

    res = emcute_pub(&topic, state_msg, sizeof(state_msg) - 1, 0);
    if (res != EMCUTE_OK) {
        printf("Error: could not publish message (%d)\n", res);
    } else {
        printf("Published: %s\n", state_msg);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };
    inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: could not connect to MQTT-SN broker (%d)\n", res);
        return 1;
    }

    printf("Connected to MQTT-SN broker at %s:%d\n", MQTT_SN_BROKER_IP, MQTT_SN_BROKER_PORT);

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    emcute_discon();
    return 0;
}
