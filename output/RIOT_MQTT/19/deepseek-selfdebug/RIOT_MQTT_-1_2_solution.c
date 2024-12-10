
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include <net/gnrc/ipv4.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define INTERVAL_MS (5 * 1000000U)

static sock_tcp_t sock;

void connect_to_broker(void) {
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = MQTT_BROKER_PORT;
    remote.family = AF_INET;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, 0) < 0) {
        puts("Error: Could not connect to MQTT broker");
        return;
    }
    puts("Connected to MQTT broker");
}

void publish_state(void) {
    char message[64];
    snprintf(message, sizeof(message), "PUBLISH %s %s\n", MQTT_TOPIC, MQTT_MESSAGE);
    if (sock_tcp_write(&sock, message, strlen(message)) < 0) {
        puts("Error: Could not send message to MQTT broker");
    } else {
        puts("State published");
    }
}

int main(void) {
    puts("Starting MQTT client");

    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
