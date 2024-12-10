#include "net/sock/udp.h"
#include "kernel_types.h"
#include "xtimer.h"
#include <stdio.h>
#include <string.h>

// Address family
#define AF_INET 2

int main(void)
{
    // Define the target server's IP and port
    const char *SERVER_IP = "47.102.103.1";
    const uint16_t SERVER_PORT = 20001;
    
    sock_udp_ep_t server = { .port = SERVER_PORT };

    // Convert IP string to byte representation
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return 1;
    }

    // Create socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create socket\n");
        return 1;
    }

    // Device state to send
    char *device_state = "work";

    // Sending message every five seconds
    while (1) {
        if (sock_udp_send(&sock, device_state, strlen(device_state), &server) < 0) {
            printf("Error: unable to send data\n");
        } else {
            printf("Data sent: %s\n", device_state);
        }
        
        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    // Close socket (unreachable in this example)
    sock_udp_close(&sock);
    return 0;
}