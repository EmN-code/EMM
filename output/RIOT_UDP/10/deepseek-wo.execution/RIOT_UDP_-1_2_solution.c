
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "net/sock.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL 5

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char buffer[10];

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error converting IP address");
        return 1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Prepare the device state message
        strncpy(buffer, DEVICE_STATE, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';

        // Send the device state message
        if (sock_udp_send(&sock, buffer, strlen(buffer), &remote) < 0) {
            puts("Error sending data");
        } else {
            printf("Sent: %s\n", buffer);
        }

        // Wait for 5 seconds before sending the next message
        sleep(INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
