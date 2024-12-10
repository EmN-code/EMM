#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883

static sock_udp_ep_t gw = {};
static sock_udp_t sock;

static void publish_state(void) {
    const char *state_msg = "work or not work";
    printf("Publishing state: %s\n", state_msg);
    sock_udp_send(&sock, state_msg, strlen(state_msg), &gw);
}

int main(void) {
    printf("MQTT Publisher with ESP32 on RIOT\n");

    // Initialize the gateway (MQTT broker) address
    gw.family = AF_INET;
    gw.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) != 1) {
        printf("Error: unable to parse broker IP\n");
        return -1;
    }

    // Create a UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return -1;
    }

    // Setup a periodic timer to publish the state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}