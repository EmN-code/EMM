
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_STATE_MSG_LEN 4
#define INTERVAL_SECONDS 5

static void send_device_state(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    inet_addr_from_str((struct in_addr *)&remote.addr.ipv4, UDP_SERVER_IP);

    sock_udp_t sock;
    sock_udp_ep_t local = { .family = AF_INET };

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Failed to create UDP socket");
        return;
    }

    if (sock_udp_send(&sock, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN, &remote) < 0) {
        puts("Failed to send UDP message");
    } else {
        puts("Device state sent successfully");
    }

    sock_udp_close(&sock);
}

int main(void)
{
    while (1) {
        send_device_state();
        xtimer_sleep(INTERVAL_SECONDS);
    }

    return 0;
}
