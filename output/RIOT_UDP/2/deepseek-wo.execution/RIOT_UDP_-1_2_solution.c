
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "net/emcute.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL 5

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;

    // Convert the server IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: invalid server IP address");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: failed to create UDP socket");
        return 1;
    }

    while (1) {
        // Send the device state to the server
        if (sock_udp_send(&sock, DEVICE_STATE, strlen(DEVICE_STATE), &remote) < 0) {
            puts("Error: failed to send data");
        } else {
            printf("Sent: %s\n", DEVICE_STATE);
        }

        // Wait for 5 seconds before sending the next message
        sleep(INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    // sock_udp_close(&sock);

    return 0;
}
