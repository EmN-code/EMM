
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define SEND_INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

static void send_udp_message(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    inet_addr_from_str((struct in_addr *)&remote.addr.ipv4, UDP_SERVER_IP);

    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;  // Let the system assign a local port

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    char buf[16];
    strncpy(buf, DEVICE_STATE_MSG, sizeof(buf));

    if (sock_udp_send(&sock, buf, strlen(buf), &remote) < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }

    sock_udp_close(&sock);
}

int main(void)
{
    while (1) {
        send_udp_message();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
