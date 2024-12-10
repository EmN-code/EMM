
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/sock/util.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    /* Convert server IP address to binary form */
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: Invalid server IP address");
        return 1;
    }

    /* Create UDP socket */
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: Failed to create UDP socket");
        return 1;
    }

    while (1) {
        /* Send device state */
        if (sock_udp_send(&sock, DEVICE_STATE, strlen(DEVICE_STATE), &remote) < 0) {
            puts("Error: Failed to send data");
        } else {
            printf("Sent: %s\n", DEVICE_STATE);
        }

        /* Wait for 5 seconds */
        usleep(SEND_INTERVAL);
    }

    /* Close the socket */
    sock_udp_close(&sock);

    return 0;
}
