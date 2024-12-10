#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <net/af.h>
#include <net/protnum.h>
#include <net/sock/udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 64
#define SEND_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    
    // Resolve the server address
    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse server address\n");
        return 1;
    }
    
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP sock\n");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    const char *state_message = "Device State: Work";

    while (1) {
        // Clear the buffer and copy the state message
        memset(buffer, 0, BUFFER_SIZE);
        strncpy(buffer, state_message, BUFFER_SIZE - 1);

        // Send the message
        if (sock_udp_send(&sock, buffer, strlen(buffer), &server) < 0) {
            printf("Error: unable to send UDP packet\n");
        } else {
            printf("UDP packet sent: %s\n", buffer);
        }

        // Wait for the specified interval
        ztimer_sleep(ZTIMER_MSEC, SEND_INTERVAL);
    }

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}