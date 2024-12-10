#include <netinet/in.h>  // For sockaddr_in and inet_pton
#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

// Function to send device state
void send_state(sock_udp_t *sock, const sock_udp_ep_t *remote, const char *state) {
    int res = sock_udp_send(sock, state, strlen(state), remote);
    if (res < 0) {
        printf("Error sending UDP packet: %d\n", res);
    } else {
        printf("Sent '%s' to %s:%d\n", state, SERVER_IP, SERVER_PORT);
    }
}

int main(void) {
    // Initialize local and remote endpoint structures
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Initialize and configure remote endpoint
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Failed to parse IPv4 address\n");
        return 1;
    }

    // Create UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket\n");
        return 1;
    }

    // Device state
    const char *device_state_work = "work";
    const char *device_state_not_work = "not work";

    while (1) {
        send_state(&sock, &remote, device_state_work);
        xtimer_sleep(5);
        send_state(&sock, &remote, device_state_not_work);
        xtimer_sleep(5);
    }

    return 0;
}