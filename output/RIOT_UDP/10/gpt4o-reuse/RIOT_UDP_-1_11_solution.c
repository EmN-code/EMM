
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFF_LEN 512

void udp_msg_sender(sock_udp_t *sock, const sock_udp_ep_t *remote)
{
    char buf[BUFF_LEN];
    while (1)
    {
        // Simulate device state
        const char *state = (rand() % 2 == 0) ? "work" : "not work";
        snprintf(buf, BUFF_LEN, "Device state: %s", state);

        printf("Sending: %s\n", buf);
        ssize_t res = sock_udp_send(sock, buf, strlen(buf), remote);
        if (res < 0) {
            printf("Failed to send UDP message\n");
        }

        xtimer_sleep(5);  // Sleep for 5 seconds
    }
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    // Initialize remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Create UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return -1;
    }

    printf("UDP client started\n");
    udp_msg_sender(&sock, &remote);

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}
