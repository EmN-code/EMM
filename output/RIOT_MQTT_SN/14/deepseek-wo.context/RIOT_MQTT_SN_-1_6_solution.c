
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <emcute.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char mqtt_sn_topic_name[CONFIG_EMCUTE_TOPIC_MAXLEN];
static emcute_topic_t mqtt_sn_topic;

static void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static int state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    printf("Publishing state: %s\n", state);

    int res = emcute_pub(&mqtt_sn_topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing state: %d\n", res);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };
    int result = inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);
    if (result != 1) {
        puts("Error: invalid broker IP address");
        return 1;
    }

    puts("Connecting to MQTT-SN broker...");
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    strncpy(mqtt_sn_topic_name, TOPIC_NAME, sizeof(mqtt_sn_topic_name));
    mqtt_sn_topic.name = mqtt_sn_topic_name;

    puts("Registering topic...");
    res = emcute_reg(&mqtt_sn_topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    puts("Starting periodic state publishing...");
    while (1) {
        publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    return 0;
}
