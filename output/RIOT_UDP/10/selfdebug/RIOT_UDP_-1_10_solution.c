#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include <stdio.h>
#include <string.h>

#define SERVER_PORT (20001)
#define SERVER_ADDR "47.102.103.1"
#define BUFFER_SIZE (128)
#define SEND_INTERVAL (5U * US_PER_SEC) // 5 seconds

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    char message[] = "Device state: work";
    uint8_t buffer[BUFFER_SIZE];

    // Convert string IP to binary format
    if (inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4) != 1) {
        puts("Error converting IP address.");
        return -1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket.");
        return -1;
    }

    while (1) {
        // Send message
        if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
            puts("Error sending message.");
        }
        else {
            puts("Message sent successfully.");
        }
        
        // Wait for the next interval
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    // Close the socket (unreachable code)
    sock_udp_close(&sock);
    return 0;
}