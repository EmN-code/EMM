
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_INTERVAL 5000000  // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t client;
static network_t network;
static uint8_t sendbuf[128];
static uint8_t readbuf[128];

void mqtt_publish_state(void) {
    mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), 0, 0);
}

int main(void) {
    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("No network interface found");
        return 1;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    IPV4_ADDR(broker_addr, 47, 102, 103, 1);
    NetworkConnect(&network, (char *)&broker_addr, MQTT_BROKER_PORT);
    MQTTPublish(&client, MQTT_TOPIC, MQTT_MESSAGE);

    // Main loop
    while (1) {
        mqtt_publish_state();
        xtimer_usleep(MQTT_MESSAGE_INTERVAL);
    }

    return 0;
}
