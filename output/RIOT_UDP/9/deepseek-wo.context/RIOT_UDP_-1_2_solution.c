
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_LEN 12
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static char device_state[DEVICE_STATE_LEN] = "work";

static void send_state(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char buf[DEVICE_STATE_LEN];

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid server address");
        return;
    }

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: failed to create UDP socket");
        return;
    }

    strncpy(buf, device_state, DEVICE_STATE_LEN);
    buf[DEVICE_STATE_LEN - 1] = '\0';  // Ensure null-termination

    if (sock_udp_send(&sock, buf, strlen(buf), NULL) < 0) {
        puts("Error: failed to send UDP packet");
    } else {
        printf("Sent: %s\n", buf);
    }

    sock_udp_close(&sock);
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        send_state();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
    }

    return 0;
}
