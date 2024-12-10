#include <xtimer.h>  // For using timers and delays
#include <stdio.h>   // Standard IO operations
#include <unistd.h>  // For sleep function
#include <net/af.h>  // For address family types
#include <net/sock/udp.h>  // For UDP socket operations

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET,
                             .port = 20001 };
    uint8_t addr[NETADDR_IPV6] = {0};
    addr[0] = 47;
    addr[1] = 102;
    addr[2] = 103;
    addr[3] = 1;
    memcpy(remote.addr.ipv6, addr, sizeof(addr));

    sock_udp_t sock;
    char device_state[16];
    int result;

    // Initialize a UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return -1;
    }

    // In a loop, send the device state every 5 seconds
    while (1) {
        // Set the device state: "work" or "not work"
        snprintf(device_state, sizeof(device_state), "work");

        // Send the device state
        result = sock_udp_send(&sock, device_state, sizeof(device_state), &remote);
        if (result < 0) {
            printf("Error sending data: %d\n", result);
        } else {
            printf("Sent: %s\n", device_state);
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    
    sock_udp_close(&sock);
    return 0;
}