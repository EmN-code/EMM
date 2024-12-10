#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/af.h"
#include "net/protnames.h"
#include "net/sock_udp.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_WORK "Device is working"
#define MESSAGE_NOT_WORK "Device is not working"
#define SLEEP_DURATION 5

int main(void) {
    // Initialize UDP
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    if (ipv6_addr_from_str((ipv6_addr_t *) &server.addr.ipv6, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse destination address\n");
        return 1;
    }

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        // Simulate device state
        int device_state = 1; // Change this to 0 to simulate "not working"

        // Prepare the message
        const char *message = device_state ? MESSAGE_WORK : MESSAGE_NOT_WORK;
        size_t message_len = strlen(message);

        // Send message via UDP
        ssize_t res = sock_udp_send(&sock, message, message_len, &server);
        if (res < 0) {
            printf("Error sending the message\n");
        } else {
            printf("Message sent: %s\n", message);
        }

        // Wait for the specified time
        xtimer_sleep(SLEEP_DURATION);
    }

    return 0;
}