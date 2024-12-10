
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void send_device_state(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    inet_addr_from_str((struct in_addr *)&remote.addr.ipv4, UDP_SERVER_IP);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &remote, 0);

    while (1) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%s", DEVICE_STATE_MSG);
        if (sock_udp_send(&sock, buf, strlen(buf), NULL) < 0) {
            puts("Error sending UDP message");
        } else {
            puts("Device state sent");
        }
        xtimer_usleep(SEND_INTERVAL);
    }

    sock_udp_close(&sock);
}

int main(void)
{
    puts("Starting UDP client");
    send_device_state();
    return 0;
}
