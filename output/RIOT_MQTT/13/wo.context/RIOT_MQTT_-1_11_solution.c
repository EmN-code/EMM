#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>  // Ensure this is included before MQTTClient.h
#include <MQTTClient.h>  // Must be the last to be included

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MSG_INTERVAL (5U * US_PER_SEC)  // 5 seconds

static char send_buffer[128];
static char read_buffer[128];
static mqtt_client_t mqtt_client;

void publish_state(mqtt_client_t *client, const char *state) {
    mqtt_publish(client, NULL, MQTT_TOPIC, state, strlen(state), MQTT_QOS_0, 0);
}

int main(void) {
    sock_udp_ep_t broker_ep = { .family = AF_INET6, .port = MQTT_BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&broker_ep.addr, MQTT_BROKER_IP);

    mqtt_client_init(&mqtt_client, &broker_ep, send_buffer, sizeof(send_buffer), read_buffer, sizeof(read_buffer));

    mqtt_connect(&mqtt_client, NULL, NULL, 0, 0, 0, 0, 0);

    while (1) {
        publish_state(&mqtt_client, "work");
        xtimer_sleep(MSG_INTERVAL);
    }

    return 0;
}