
#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"  // Include the appropriate header for RIOT's UDP socket API
#include "xtimer.h"        // Include for timer functionality

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFF_LEN 512

void udp_msg_sender(sock_udp_t *sock, const sock_udp_ep_t *remote)
{
    char buf[BUFF_LEN];
    while (1)
    {
        // Simulate device state: "work" or "not work"
        const char *state = (rand() % 2 == 0) ? "work" : "not work";
        snprintf(buf, BUFF_LEN, "Device state: %s", state);

        printf("Sending: %s\n", buf);  // Print the message to be sent

        // Send the message to the server
        if (sock_udp_send(sock, buf, strlen(buf), remote) < 0) {
            puts("Error sending UDP message");
        }

        xtimer_sleep(5);  // Sleep for 5 seconds before sending the next message
    }
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    // Initialize the remote endpoint
    if (sock_udp_str2ep(&remote, SERVER_IP, SERVER_PORT) < 0) {
        puts("Error initializing remote endpoint");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Start sending messages
    udp_msg_sender(&sock, &remote);

    // Close the socket (this line will never be reached due to the infinite loop)
    sock_udp_close(&sock);

    return 0;
}
