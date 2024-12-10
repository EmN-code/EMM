
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/af.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
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
    mqtt_client_connect(&mqtt_client, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_CLIENT_ID, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
}

static void mqtt_publish(void)
{
    mqtt_publish_t pubmsg;
    pubmsg.topic = MQTT_TOPIC;
    pubmsg.payload = MQTT_MESSAGE;
    pubmsg.payload_len = strlen(MQTT_MESSAGE);
    pubmsg.qos = MQTT_QOS;
    pubmsg.retain = MQTT_RETAIN;

    mqtt_publish(&mqtt_client, &pubmsg);
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
    local.port = 0;
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Connect to MQTT broker
    mqtt_connect();

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
