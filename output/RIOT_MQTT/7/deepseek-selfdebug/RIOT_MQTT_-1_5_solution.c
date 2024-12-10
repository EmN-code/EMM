
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/tcp.h>
#include <net/sock/util.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t sock;
static sock_tcp_ep_t remote;

void publish_state(void) {
    char message[64];
    snprintf(message, sizeof(message), "PUBLISH %s %s\n", MQTT_TOPIC, MQTT_MESSAGE);

    if (sock_tcp_send(&sock, message, strlen(message)) < 0) {
        puts("Failed to send message");
    } else {
        puts("State published");
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize remote endpoint
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    // Create TCP socket
    if (sock_tcp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Failed to create socket");
        return 1;
    }

    // Connect to MQTT broker
    if (sock_tcp_connect(&sock, &remote, 0) < 0) {
        puts("Failed to connect to MQTT broker");
        sock_tcp_close(&sock);
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    // Close the socket
    sock_tcp_close(&sock);
    return 0;
}
