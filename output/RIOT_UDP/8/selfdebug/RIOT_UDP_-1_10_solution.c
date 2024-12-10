#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 128
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    uint8_t buf[BUFFER_SIZE];
    sock_udp_ep_t remote;
    sock_udp_t sock;
    int res;
    const char *device_state = "work"; // Use "not work" as needed.

    // Initialize remote endpoint structure
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        puts("Error: unable to convert IP address.");
        return 1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket.");
        return 1;
    }

    // Main loop to send UDP messages every five seconds
    while (1) {
        // Send device state
        ssize_t bytes_sent = sock_udp_send(&sock, device_state, strlen(device_state), &remote);
        if (bytes_sent < 0) {
            printf("Error: unable to send data. Error code: %d\n", (int)bytes_sent);
        }

        // Wait for the next send interval
        xtimer_sleep(SEND_INTERVAL);
    }

    // Clean up the socket (not reached in this infinite loop example)
    sock_udp_close(&sock);
    return 0;
}