// Include the necessary header files in descending alphabetical order by their first letter
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "net/sock/tcp.h"
#include "net/sock/udp.h"
#include "xtimer.h"

// Define the MQTT broker address and port
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883

// Define the message to publish and the publication interval
#define PUB_MESSAGE "work"
#define PUB_INTERVAL (5U * US_PER_SEC)

// Main function
int main(void) {
    // Initialize the TCP socket to connect to the broker
    sock_tcp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);
    sock_tcp_ep_connect(&sock, &remote);

    while (1) {
        // Publish state "work" to the MQTT broker
        char msg[128];
        snprintf(msg, sizeof(msg), "{\"state\": \"%s\"}", PUB_MESSAGE);
        sock_tcp_send(&sock, msg, strlen(msg));

        // Wait for the next publication interval
        xtimer_sleep(PUB_INTERVAL / US_PER_SEC);
    }

    sock_tcp_close(&sock);

    return 0;
}