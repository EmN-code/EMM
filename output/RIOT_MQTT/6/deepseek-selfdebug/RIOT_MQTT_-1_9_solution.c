
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t sock;
static mqtt_client_t client;
static char sendbuf[128];
static char readbuf[128];

void publish_state(void) {
    mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), 0, 0);
}

int main(void) {
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    struct in_addr broker_addr;

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    remote.port = MQTT_BROKER_PORT;
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.addr.ipv4 = broker_addr;

    // Initialize MQTT client
    mqtt_client_init(&client, &sock, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf), NULL);

    // Connect to MQTT broker
    if (mqtt_connect(&client, &remote, MQTT_CLIENT_ID, NULL, NULL, 0, 0, 0, 0) != 0) {
        puts("MQTT connection failed");
        return 1;
    }

    puts("Connected to MQTT broker");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
