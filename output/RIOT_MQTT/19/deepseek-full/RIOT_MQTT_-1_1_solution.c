
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
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t mqtt_client;
static uint8_t sendbuf[128];
static uint8_t readbuf[128];

static void mqtt_connect(void)
{
    mqtt_client_init(&mqtt_client);

    mqtt_connect_info_t connect_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
    };

    mqtt_client_connect(&mqtt_client, &connect_info);
}

static void mqtt_publish(void)
{
    mqtt_publish_info_t publish_info = {
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = strlen(MQTT_MESSAGE),
    };

    mqtt_client_publish(&mqtt_client, &publish_info);
}

int main(void)
{
    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("No network interface found");
        return 1;
    }

    // Initialize UDP socket
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Failed to create UDP socket");
        return 1;
    }

    // Connect to MQTT broker
    mqtt_connect();

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Clean up
    sock_udp_close(&sock);
    return 0;
}
