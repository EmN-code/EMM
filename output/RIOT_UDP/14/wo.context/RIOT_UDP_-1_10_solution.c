#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <arpa/inet.h>  // Correctly include the necessary header for inet_pton
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL (5U * 1000000U) // 5 seconds

int main(void) {
    sock_udp_ep_t remote;
    char *message = "work";
    sock_udp_t sock;

    // Set the remote server's address
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    // Convert server IP address to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: could not convert IP address");
        return -1;
    }

    // Create a UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: could not create socket");
        return -1;
    }

    // Continuous loop to send data every 5 seconds
    while (1) {
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &remote);
        if (res < 0) {
            puts("Error sending message");
        } else {
            puts("Message sent: work");
        }

        // Wait for the next interval
        xtimer_usleep(MESSAGE_INTERVAL);
    }

    return 0;
}