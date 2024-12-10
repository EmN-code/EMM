#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // 5 seconds

int main(void) {
    /* Application logic that periodically publishes a state to the MQTT broker */
    printf("Starting MQTT state publisher example using ESP32\n");

    /* Initialization and socket setup */
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    uint8_t state_message[] = "work";
    int bytes_sent;

    if(inet_pton(AF_INET, MQTT_BROKER_ADDRESS, &remote.addr.ipv6) != 1) {
        printf("Error: cannot convert broker address\n");
        return 1;
    }

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        /* Publish the state message */
        bytes_sent = sock_udp_send(&sock, state_message, sizeof(state_message), &remote);
        if (bytes_sent <= 0) {
            printf("Error sending message to the broker\n");
        } else {
            printf("Message sent: %s\n", state_message);
        }

        /* Wait for the next publish interval */
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}