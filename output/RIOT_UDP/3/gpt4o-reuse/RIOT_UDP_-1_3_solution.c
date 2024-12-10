
#include <stdio.h>
#include <string.h>
#include "xtimer.h"  // Include for xtimer functions
#include "net/sock/udp.h"  // Include for UDP socket functions

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFF_LEN 512

void udp_msg_sender(sock_udp_t *sock, const sock_udp_ep_t *remote)
{
    char buf[BUFF_LEN];
    while (1)
    {
        strcpy(buf, "work");  // Simulate device state
        printf("client: %s\n", buf);  // Print the message to be sent
        sock_udp_send(sock, buf, strlen(buf), remote);  // Send the message
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Create the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return -1;
    }

    // Start sending messages
    udp_msg_sender(&sock, &remote);

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}
