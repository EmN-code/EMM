
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFF_LEN 512

void udp_msg_sender(int fd, struct sockaddr_in *dst)
{
    socklen_t len = sizeof(*dst);
    char buf[BUFF_LEN];

    while (1) {
        memset(buf, 0, BUFF_LEN);
        strcpy(buf, "work"); // or "not work" depending on the device state
        printf("client: %s\n", buf);  // Print the message to be sent
        sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)dst, len);
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return -1;
    }

    udp_msg_sender(sock.fd, (struct sockaddr_in *)&remote);

    sock_udp_close(&sock);

    return 0;
}
