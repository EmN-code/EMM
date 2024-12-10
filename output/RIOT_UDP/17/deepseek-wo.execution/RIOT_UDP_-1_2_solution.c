
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL 5

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    char buffer[20];

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;

    // Convert the server IP address from string to binary form
    if (sock_ipaddr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP) < 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    // Initialize the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: failed to create UDP socket");
        return 1;
    }

    while (1) {
        // Prepare the device state message
        strncpy(buffer, DEVICE_STATE, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';

        // Send the device state message to the server
        ssize_t bytes_sent = sock_udp_send(&sock, buffer, strlen(buffer), &remote);
        if (bytes_sent < 0) {
            puts("Error: failed to send UDP message");
        } else {
            printf("Sent %zd bytes: %s\n", bytes_sent, buffer);
        }

        // Wait for 5 seconds before sending the next message
        sleep(SEND_INTERVAL);
    }

    // Close the UDP socket
    sock_udp_close(&sock);

    return 0;
}
