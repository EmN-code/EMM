
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define MQTT_SN_TOPIC "state"
#define MQTT_SN_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char mqtt_sn_topic_name[] = MQTT_SN_TOPIC;
static emcute_topic_t mqtt_sn_topic;

static void publish_state(void)
{
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    printf("Publishing state: %s\n", state);

    int res = emcute_pub(&mqtt_sn_topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish state: %d\n", res);
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };
    inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);

    printf("Connecting to MQTT-SN broker at %s:%d\n", MQTT_SN_BROKER_IP, MQTT_SN_BROKER_PORT);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return 1;
    }

    printf("Connected to MQTT-SN broker\n");

    mqtt_sn_topic.name = mqtt_sn_topic_name;
    res = emcute_reg(&mqtt_sn_topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    printf("Topic registered with ID: %u\n", mqtt_sn_topic.id);

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
