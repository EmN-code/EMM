
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "net/emcute.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL 5

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    char buffer[256];

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: Invalid address");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: Failed to create UDP socket");
        return 1;
    }

    while (1) {
        // Prepare the message
        snprintf(buffer, sizeof(buffer), "Device state: %s", DEVICE_STATE);

        // Send the message
        if (sock_udp_send(&sock, buffer, strlen(buffer), &remote) < 0) {
            puts("Error: Failed to send message");
        } else {
            printf("Message sent: %s\n", buffer);
        }

        // Wait for 5 seconds before sending the next message
        sleep(INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}