
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/mqtt.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void _mqtt_connect(sock_udp_t *sock, const sock_udp_ep_t *remote)
{
    // Placeholder for MQTT connection logic
    printf("Connecting to MQTT broker at %s:%d\n", SERVER_IP, SERVER_PORT);
    // Implement MQTT connection logic here
}

static void _mqtt_publish(sock_udp_t *sock, const sock_udp_ep_t *remote)
{
    // Placeholder for MQTT publish logic
    printf("Publishing to topic %s: %s\n", TOPIC, PAYLOAD);
    // Implement MQTT publish logic here
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    _mqtt_connect(&sock, &remote);

    while (1) {
        _mqtt_publish(&sock, &remote);
        xtimer_usleep(INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}
