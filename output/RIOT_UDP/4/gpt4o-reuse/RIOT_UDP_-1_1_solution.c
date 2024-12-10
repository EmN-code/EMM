
#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFF_LEN 512
#define DEVICE_STATE "work"  // or "not work"

void udp_msg_sender(sock_udp_ep_t *remote)
{
    sock_udp_t sock;
    char buf[BUFF_LEN];

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    while (1) {
        // Prepare the message
        snprintf(buf, BUFF_LEN, "Device state: %s", DEVICE_STATE);

        // Send the message
        if (sock_udp_send(&sock, buf, strlen(buf), NULL) < 0) {
            puts("Error sending UDP message");
        } else {
            printf("Sent: %s\n", buf);
        }

        // Wait for 5 seconds before sending the next message
        xtimer_sleep(5);
    }

    // Close the socket
    sock_udp_close(&sock);
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    // Set up the remote endpoint
    remote.port = SERVER_PORT;
    if (sock_udp_str2ep(&remote, SERVER_IP) < 0) {
        puts("Error parsing server address");
        return 1;
    }

    // Start sending messages
    udp_msg_sender(&remote);

    return 0;
}
