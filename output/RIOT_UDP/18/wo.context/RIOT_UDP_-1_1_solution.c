#include <stdio.h>
#include <string.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/netif.h>
#include <errno.h>
#include <xtimer.h>

int main(void) {
    // Define the device state
    const char *state_work = "work";
    const char *state_not_work = "not work";

    // Choose the desired state
    const char *device_state = state_work; // or state_not_work

    // UDP address and port information
    const char *server_ip = "47.102.103.1";
    uint16_t server_port = 20001;

    // Create a UDP socket
    sock_udp_ep_t server = { .family = AF_INET6, .port = server_port };
    sock_udp_t sock;

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    // Resolve the server address
    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr, server_ip) == NULL) {
        printf("Error parsing server IP address\n");
        return 1;
    }

    // Send the device state every 5 seconds
    while (1) {
        int res = sock_udp_send(&sock, device_state, strlen(device_state), &server);
        if (res < 0) {
            printf("Error sending UDP packet: %d\n", res);
        } else {
            printf("Sent device state: %s\n", device_state);
        }
        // Delay for 5 seconds
        xtimer_sleep(5);
    }

    // Close the socket (never reached due to infinite loop)
    sock_udp_close(&sock);

    return 0;
}