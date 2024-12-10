#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>

int main(void) {
    const char *server_ip = "47.102.103.1";
    uint16_t server_port = 20001;
    char* device_state = "work";
    sock_udp_ep_t server = { .family = AF_INET, .port = server_port };
    sock_udp_t sock;

    // Create a socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: Cannot create socket");
        return 1;
    }

    // Convert server IP to server address
    if (inet_pton(AF_INET, server_ip, &server.addr.ipv4) != 1) {
        puts("Error: Invalid server address");
        return 1;
    }

    while (1) {
        // Send device state to server
        if (sock_udp_send(&sock, device_state, strlen(device_state), &server) < 0) {
            puts("Error: Failed to send message");
        } else {
            printf("Sent: %s\n", device_state);
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}