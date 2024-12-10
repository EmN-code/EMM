#include <string.h>
#include <xtimer.h>
#include <net/inet/pton.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * 1000000U)  // interval in microseconds

int main(void) {
    /* Definitions */
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    char device_state[] = "work";  // Change to "not work" as needed
    uint8_t buffer[64];

    /* Create UDP sock */
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP sock");
        return -1;
    }

    /* Convert server IP address */
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse IPv4 address");
        return -1;
    }

    /* Main loop to send device state */
    while (1) {
        memcpy(buffer, device_state, strlen(device_state));
        if (sock_udp_send(&sock, buffer, strlen(device_state), &remote) < 0) {
            puts("Error sending UDP packet");
        } else {
            puts("Device state sent");
        }
        xtimer_sleep(5);
    }

    return 0;
}