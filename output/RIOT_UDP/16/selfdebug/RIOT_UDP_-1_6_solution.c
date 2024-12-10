#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SLEEP_INTERVAL (5U * US_PER_SEC)  // Sleep for 5 seconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char *message = "Device state: work";
    int res;

    // Initialize the server address
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        puts("Error: unable to parse IP address");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    while (1) {
        // Send the device state message
        res = sock_udp_send(&sock, message, strlen(message), &remote);
        if (res < 0) {
            printf("Error sending message: %d\n", res);
        } else {
            printf("Sent message: %s\n", message);
        }

        // Wait for 5 seconds
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}