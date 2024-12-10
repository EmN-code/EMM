#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/sock/udp.h"
#include "net/af.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U)
#define BUFFER_SIZE 128

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    sock_udp_t sock;
    uint8_t buffer[BUFFER_SIZE];
    int state = 0; // 0: not working, 1: working

    // Parse the server address
    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse the server address\n");
        return 1;
    }

    // Open the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, SOCK_FLAGS_REUSE_EP) < 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }

    while (1) {
        // Toggle state between 0 "not work" and 1 "work"
        state = (state == 0) ? 1 : 0;
        snprintf((char*)buffer, BUFFER_SIZE, "Device state: %s", state == 0 ? "not work" : "work");

        printf("Sending: %s\n", buffer);

        // Send the buffer to the server
        if (sock_udp_send(&sock, buffer, strlen((char *)buffer), &server) < 0) {
            printf("Error: unable to send data\n");
        } else {
            printf("Data sent successfully\n");
        }

        // Wait for the specified interval
        xtimer_sleep(INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}