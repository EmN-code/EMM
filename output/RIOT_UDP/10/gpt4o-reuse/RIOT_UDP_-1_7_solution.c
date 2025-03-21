
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFF_LEN 512

void udp_msg_sender(int fd, sock_udp_ep_t *dst)
{
    while (1) {
        char buf[BUFF_LEN] = "work";  // Example device state
        printf("client: %s\n", buf);  // Print the message to be sent
        sock_udp_send(fd, buf, strlen(buf), dst);
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    // Initialize the remote endpoint
    if (sock_udp_str2ep(&remote, SERVER_IP, SERVER_PORT) < 0) {
        puts("Error parsing remote address");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    udp_msg_sender(sock, &remote);

    sock_udp_close(&sock);

    return 0;
}
