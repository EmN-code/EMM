#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/af.h>
#include <net/sock.h>
#include <net/conn/ip.h>
#include <mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL 5

static char *states[2] = {"work", "not work"};

static void publish_state(mqtt_client_t *client) {
    static int state_index = 0;
    const char *topic = "device/state";

    while (1) {
        mqtt_publish(client, NULL, topic, states[state_index], strlen(states[state_index]), MQTT_QOS_0, 0);
        state_index = (state_index + 1) % 2;
        xtimer_sleep(PUBLISH_INTERVAL);
    }
}

int main(void) {
    mqtt_client_t client;
    sock_udp_ep_t broker_ep = {
        .family = AF_INET,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = BROKER_PORT
    };
    ipv4_addr_t ipv4_broker_ip;
    ipv4_addr_from_str(&ipv4_broker_ip, BROKER_IP);
    memcpy(&broker_ep.addr.ipv4, &ipv4_broker_ip, sizeof(ipv4_addr_t));

    mqtt_init(&client, &broker_ep, "client_id", NULL, NULL, 0);

    if (mqtt_connect(&client, true, NULL, NULL, 0, 0, 0, 0) != MQTT_CONNECT_ACCEPTED) {
        printf("Failed to connect to the broker\n");
        return 1;
    }
    printf("Connected to the broker\n");

    publish_state(&client);

    return 0;
}