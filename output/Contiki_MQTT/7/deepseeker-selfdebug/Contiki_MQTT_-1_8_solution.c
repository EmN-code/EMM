
#include <stdio.h>
#include <string.h>
#include "net/ipv6/simple-udp.h"
#include "net/mqtt.h"
#include "xtimer.h"

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_PAYLOAD_SIZE 128

static struct mqtt_connection conn;
static char payload[MQTT_PAYLOAD_SIZE];

static void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    snprintf(payload, MQTT_PAYLOAD_SIZE, "%s", states[state_index]);
    state_index = (state_index + 1) % 2;

    mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_0, MQTT_RETAIN_OFF, 0, MQTT_TOPIC_ALIAS_EN_OFF, NULL);
}

int main(void) {
    mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, 60, MQTT_CLEAN_SESSION_ON, NULL);

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
