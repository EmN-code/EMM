
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_STATE_MSG_LEN 4
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static void send_udp_message(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    sock_udp_t sock;
    char buf[DEVICE_STATE_MSG_LEN];

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    strncpy(buf, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN);
    if (sock_udp_send(&sock, buf, DEVICE_STATE_MSG_LEN, NULL) < 0) {
        puts("Error sending UDP message");
    } else {
        puts("UDP message sent successfully");
    }

    sock_udp_close(&sock);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        send_udp_message();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
    }

    return 0;
}
