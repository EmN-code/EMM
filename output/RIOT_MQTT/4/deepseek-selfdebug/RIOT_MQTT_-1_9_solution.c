
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static void publish_state(sock_udp_t *sock, const sock_udp_ep_t *remote)
{
    static const char *state = "work";
    static const char *topic = "state";
    char buf[128];
    int len = snprintf(buf, sizeof(buf), "PUBLISH %s %s\n", topic, state);

    if (sock_udp_send(sock, buf, len, remote) < 0) {
        puts("Error sending message");
    } else {
        puts("State published");
    }
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    local.port = 0; /* Use any available port */
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        publish_state(&sock, &remote);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}
