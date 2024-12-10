
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

static sock_udp_ep_t broker_ep = { .family = AF_INET, .port = MQTT_BROKER_PORT };
static sock_udp_t sock;
static mqtt_client_t client;
static network_t network;
static unsigned char sendbuf[1024];
static unsigned char readbuf[1024];

void publish_state(void)
{
    mqtt_message_t message;
    message.topic = MQTT_TOPIC;
    message.payload = MQTT_MESSAGE;
    message.payloadlen = MQTT_MESSAGE_LEN;
    message.qos = MQTT_QOS_0;
    message.retained = 0;
    message.dup = 0;

    mqtt_publish(&client, &message);
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
    sock_udp_ep_t local_ep = { .family = AF_INET, .port = 0 };
    if (sock_udp_create(&sock, &local_ep, NULL, 0) < 0) {
        puts("Failed to create UDP socket");
        return 1;
    }

    // Set broker endpoint
    ipv4_addr_t broker_addr = IPV4_ADDR(47, 102, 103, 1);
    broker_ep.addr.ipv4 = broker_addr;

    // Initialize MQTT client
    mqtt_client_init(&client, &network, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    if (mqtt_connect(&client, &broker_ep, MQTT_CLIENT_ID, NULL, NULL, 0, 0) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Disconnect from MQTT broker
    mqtt_disconnect(&client);

    return 0;
}
