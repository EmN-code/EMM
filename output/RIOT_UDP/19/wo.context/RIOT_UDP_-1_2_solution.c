#include <xtimer.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char server_addr[16];

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        puts("Error: unable to convert IP address from text to binary form.");
        return -1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket.");
        return -1;
    }

    while (1) {
        // Send message to server
        if (sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &server) < 0) {
            puts("Error: unable to send message.");
        } else {
            puts("Message sent: work");
        }

        // Wait for 5 seconds
        xtimer_sleep(INTERVAL);
    }

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}
