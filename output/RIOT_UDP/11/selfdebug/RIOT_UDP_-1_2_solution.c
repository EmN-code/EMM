#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE     "work"
#define INTERVAL    (5U * US_PER_SEC) // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };

    // Convert the IP address from string to the byte representation
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    // Create the UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Repeatedly send message every INTERVAL seconds
    while (1) {
        ssize_t res = sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &server);
        if (res < 0) {
            puts("Error sending UDP message");
        } else {
            puts("Message sent successfully");
        }
        
        // Sleep for the defined interval
        xtimer_sleep(INTERVAL);
    }

    // Close the socket (unreachable code, just for completeness)
    sock_udp_close(&sock);
    return 0;
}