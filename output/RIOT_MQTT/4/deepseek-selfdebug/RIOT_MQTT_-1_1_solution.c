
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/ipv4/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD_WORK "work"
#define PAYLOAD_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t client;
static network_t network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void publish_state(void)
{
    static int state = 0;
    const char *payload = state ? PAYLOAD_WORK : PAYLOAD_NOT_WORK;
    state = !state;

    mqtt_publish(&client, TOPIC, payload, strlen(payload), 0, 0);
}

int main(void)
{
    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("No network interface found");
        return 1;
    }

    // Initialize MQTT client
    mqtt_client_init(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Failed to create UDP socket");
        return 1;
    }

    network_udp_create(&network, &sock);

    if (mqtt_connect(&client, "RIOT_MQTT_CLIENT", 60, 0, 0, 0, 0) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    puts("Connected to MQTT broker");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    // Disconnect from MQTT broker
    mqtt_disconnect(&client);
    sock_udp_close(&sock);

    return 0;
}
