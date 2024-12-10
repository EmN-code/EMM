
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/mqtt_client.h"
#include "net/netdev.h"
#include "net/netdev/eth.h"
#include "net/netif.h"

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"

static struct mqtt_connection conn;
static char payload[10];

void publish_state(void) {
    static uint16_t mid = 0;
    strcpy(payload, MQTT_MESSAGE);
    mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_0, MQTT_RETAIN_OFF, 0, MQTT_TOPIC_ALIAS_EN_OFF, NULL);
}

int main(void) {
    printf("Starting MQTT client...\n");

    // Initialize MQTT connection
    mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, 60, MQTT_CLEAN_SESSION_ON, NULL);

    // Set username and password if required
    // mqtt_set_username_password(&conn, "username", "password");

    // Set last will if required
    // mqtt_set_last_will(&conn, "last/will/topic", "offline", MQTT_QOS_0, NULL);

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
