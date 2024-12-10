
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL_SEC 5

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t state_buf[10];
    struct in_addr server_addr;

    // Initialize the UDP socket
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Convert the server IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr) <= 0) {
        puts("Error converting IP address");
        return 1;
    }
    remote.addr.ipv4 = server_addr;

    // Main loop to send device state every 5 seconds
    while (1) {
        // Prepare the device state message
        strncpy((char *)state_buf, DEVICE_STATE, sizeof(state_buf) - 1);
        state_buf[sizeof(state_buf) - 1] = '\0';

        // Send the device state to the server
        if (sock_udp_send(&sock, state_buf, strlen((char *)state_buf), &remote) < 0) {
            puts("Error sending UDP packet");
        } else {
            printf("Sent: %s\n", state_buf);
        }

        // Wait for 5 seconds before sending the next message
        xtimer_sleep(INTERVAL_SEC);
    }

    // Close the UDP socket
    sock_udp_close(&sock);

    return 0;
}
