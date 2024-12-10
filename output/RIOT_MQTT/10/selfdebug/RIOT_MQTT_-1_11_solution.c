#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"

// Function to simulate publishing state to an MQTT broker
static void publish_state(void) {
    sock_udp_ep_t broker_endpoint;
    memset(&broker_endpoint, 0, sizeof(broker_endpoint));
    
    broker_endpoint.family = AF_INET6;  // Assume IPv6 is used
    ipv6_addr_from_str((ipv6_addr_t*)&broker_endpoint.addr, "47.102.103.1");
    broker_endpoint.port = 1883;

    // Assume the message format suitable for MQTT over UDP
    const char *message = "work or not work";

    while (1) {
        sock_udp_t sock;
        sock_udp_create(&sock, &broker_endpoint, NULL, 0);

        printf("Publishing state to MQTT broker...\n");
        sock_udp_send(&sock, message, strlen(message), &broker_endpoint);

        sock_udp_close(&sock);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
}

int main(void) {
    printf("Starting MQTT state publisher...\n");

    // Start the publishing loop
    publish_state();

    return 0;
}