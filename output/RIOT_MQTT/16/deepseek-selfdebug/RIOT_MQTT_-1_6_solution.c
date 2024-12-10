
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote;

void publish_state(void) {
    char buf[64];
    int len = snprintf(buf, sizeof(buf), "{\"state\":\"%s\"}", MQTT_MESSAGE);

    if (sock_udp_send(&sock, buf, len, &remote) < 0) {
        puts("Error sending message");
    } else {
        puts("State published");
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize the UDP socket
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Set up the remote endpoint
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, MQTT_BROKER_IP) == NULL) {
        puts("Error parsing broker IP address");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    // Clean up
    sock_udp_close(&sock);
    return 0;
}
