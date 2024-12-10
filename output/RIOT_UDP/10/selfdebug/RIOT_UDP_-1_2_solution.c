#include <xtimer.h>
#include <stdio.h>
#include <sock/udp.h>
#include <net/af.h>
#include <kernel.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval.

static sock_udp_ep_t server = {
    .family = AF_INET,
    .port = SERVER_PORT
};

int main(void) {
    int res;
    char *device_state = "work";  // Example device state
    struct in_addr addr;

    // Convert IP address from string to binary form
    res = inet_pton(AF_INET, SERVER_IP, &addr);
    if (res <= 0) {
        printf("Error: unable to set up server IP\n");
        return 1;
    }

    // Set the server address
    memcpy(&server.addr.ipv4, &addr, sizeof(addr));

    // UDP socket setup
    sock_udp_t sock;
    res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res != 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }

    while (1) {
        // Send the device state to the server
        res = sock_udp_send(&sock, device_state, strlen(device_state), &server);
        if (res < 0) {
            printf("Error: unable to send data\n");
        } else {
            printf("Sent: %s\n", device_state);
        }

        // Wait for specified interval
        xtimer_usleep(INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}