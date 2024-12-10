
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_STATE_MSG_LEN (sizeof(DEVICE_STATE_MSG) - 1)
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static void send_device_state(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    sock_udp_t sock;
    char buf[DEVICE_STATE_MSG_LEN];

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Failed to create UDP socket");
        return;
    }

    strncpy(buf, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN);

    if (sock_udp_send(&sock, buf, DEVICE_STATE_MSG_LEN, &remote) < 0) {
        puts("Failed to send UDP message");
    } else {
        puts("Device state sent");
    }

    sock_udp_close(&sock);
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        send_device_state();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
    }

    return 0;
}
