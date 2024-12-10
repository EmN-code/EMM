
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <stdio.h>
#include <unistd.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL_SEC 5

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    char buffer[10];

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;

    // Convert the server IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid address/ Address not supported");
        return 1;
    }

    // Create a UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Failed to create UDP socket");
        return 1;
    }

    while (1) {
        // Prepare the device state message
        snprintf(buffer, sizeof(buffer), "%s", DEVICE_STATE);

        // Send the device state message to the server
        if (sock_udp_send(&sock, buffer, sizeof(buffer), &remote) < 0) {
            puts("Failed to send data");
        } else {
            printf("Sent: %s\n", buffer);
        }

        // Wait for 5 seconds before sending the next message
        sleep(INTERVAL_SEC);
    }

    // Close the socket (this part is unreachable in this example due to the infinite loop)
    sock_udp_close(&sock);

    return 0;
}
