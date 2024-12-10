#include "net/sock/udp.h"
#include "sys/xtimer.h"
#include "net/af.h"
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL 5

int main(void) {
    // Define socket and UDP endpoint
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    
    // Convert IP address from string
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: Could not convert server IP address.");
        return 1;
    }

    // Create a UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: Could not create UDP socket.");
        return 1;
    }

    // Message to send
    char *state_message = "work";  // or "not work"

    while (1) {
        // Send the message to the UDP server
        if (sock_udp_send(&sock, state_message, strlen(state_message), &remote) < 0) {
            printf("Error: Could not send message %s\n", state_message);
        } else {
            printf("Message sent: %s\n", state_message);
        }
        
        // Wait for the specified interval
        xtimer_sleep(MESSAGE_INTERVAL);
    }

    // Close the socket
    sock_udp_close(&sock);
    return 0;
}