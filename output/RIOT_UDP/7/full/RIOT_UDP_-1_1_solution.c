#include <xtimer.h>
#include <net/af.h>
#include <net/gnrc/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

int main(void) {
    kernel_pid_t iface = 6; // assuming interface 6, change as per your setup
    gnrc_netif_t *netif = gnrc_netif_get_by_pid(iface);
    gnrc_ipv6_addr_t addr;

    // Set the server address
    inet_pton(AF_INET6, SERVER_IP, &addr.u8);

    // Prepare the destination endpoint
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT, .addr = {.ipv6 = addr} };
    sock_udp_ep_t local = { .family = AF_INET6, .port = 0 };  // use any available port locally

    sock_udp_t sock;
    size_t state_len = strlen(DEVICE_STATE);

    // Create the UDP socket
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        printf("Error creating socket\n");
        return -1;
    }

    while (1) {
        // Attempt to send the device state
        if (sock_udp_send(&sock, DEVICE_STATE, state_len, &server) < 0) {
            printf("Error sending data\n");
        } else {
            printf("State sent: %s\n", DEVICE_STATE);
        }
        // Sleep for the specified interval
        xtimer_usleep(INTERVAL);
    }

    return 0;
}