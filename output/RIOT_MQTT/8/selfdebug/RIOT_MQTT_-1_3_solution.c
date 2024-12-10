#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

static const char *MQTT_BROKER_IP = "47.102.103.1";
static const uint16_t MQTT_BROKER_PORT = 1883;
static sock_udp_ep_t remote;

void publish_state(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);
    remote.port = MQTT_BROKER_PORT;

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    const char *state_message = "work";
    printf("Publishing state to broker: %s\n", state_message);
    if (sock_udp_send(&sock, state_message, strlen(state_message), &remote) < 0) {
        puts("Failed to send state message");
    }

    sock_udp_close(&sock);
}

int main(void) {
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }
    return 0;
}