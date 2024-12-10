
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <arpa/inet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5000000  // 5 seconds in microseconds

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: Invalid address/ Address not supported");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: Could not create UDP socket");
        return 1;
    }

    char message[20];
    while (1) {
        // Simulate device state
        const char *state = (rand() % 2 == 0) ? "work" : "not work";
        snprintf(message, sizeof(message), "Device state: %s", state);

        // Send the message
        if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
            puts("Error: Could not send message");
        } else {
            printf("Sent: %s\n", message);
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}
