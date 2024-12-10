
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t mqtt_client;
static char sendbuf[128];
static char readbuf[128];

void mqtt_connect(void) {
    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
    };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    mqtt_client_init(&mqtt_client, (struct sockaddr *)&broker_addr, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    mqtt_connect(&mqtt_client, MQTT_CLIENT_ID, NULL, NULL, 0);
}

void mqtt_publish(void) {
    mqtt_publish(&mqtt_client, MQTT_TOPIC, MQTT_MESSAGE, MQTT_MESSAGE_LEN, 0, 0);
}

int main(void) {
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("No network interface found");
        return 1;
    }

    // Initialize the network interface
    netif_set_default(netif);
    netif_set_up(netif);

    // Connect to the MQTT broker
    mqtt_connect();

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
