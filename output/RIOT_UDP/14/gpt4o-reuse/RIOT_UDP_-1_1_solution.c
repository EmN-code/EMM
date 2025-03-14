
#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFF_LEN 512
#define DELAY_MS 5000000  // 5 seconds in microseconds

void udp_msg_sender(sock_udp_ep_t *remote)
{
    sock_udp_t sock;
    char buf[BUFF_LEN];
    int res;

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    while (1) {
        // Prepare the message
        strcpy(buf, "work");  // Assuming "work" means the device is working
        printf("Sending: %s\n", buf);

        // Send the message
        res = sock_udp_send(&sock, buf, strlen(buf), remote);
        if (res < 0) {
            puts("Error sending UDP message");
        }

        // Wait for 5 seconds
        xtimer_usleep(DELAY_MS);
    }

    // Close the socket
    sock_udp_close(&sock);
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    // Set up the remote endpoint
    remote.port = SERVER_PORT;
    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Start sending messages
    udp_msg_sender(&remote);

    return 0;
}
