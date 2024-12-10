#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqtt.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/emac.h>
#include <xtimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)
#define MQTT_TOPIC "device/state"

static mqtt_client_t mqtt_client;
static sock_udp_ep_t broker_ep = { .family = AF_INET6, .port = BROKER_PORT };
static char client_id[] = "esp32_mqtt_client";

static void on_pub(void *arg, uint16_t pkt_id, const uint8_t *topic, size_t topic_len, const uint8_t *data, size_t data_len) {
    (void)arg;
    printf("Message with ID %d published successfully!\n", pkt_id);
}

void publish_state(void) {
    static const char *state_msg = "work";
    while (1) {
        printf("Publishing state...\n");
        mqtt_publish(&mqtt_client, MQTT_TOPIC, state_msg, strlen(state_msg), 0);
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }
}

int main(void) {
    printf("Connecting to MQTT broker...\n");
    broker_ep.netif = SOCK_ADDR_ANY_NETIF;
    ipv6_addr_from_str((ipv6_addr_t *)&broker_ep.addr.ipv6, BROKER_IP);

    mqtt_client.con_pkt_id = 1;
    mqtt_client.last_mid = 1;

    mqtt_connect(&mqtt_client, &broker_ep, client_id, true, NULL, NULL, 0, 0, on_pub);

    publish_state();

    return 0;
}