
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static void send_state(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_addr_from_str((struct in_addr *)&remote.addr.ipv4, SERVER_IP);

    char buf[32];
    snprintf(buf, sizeof(buf), "Device state: %s", DEVICE_STATE);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &remote, 0);
    sock_udp_send(&sock, buf, strlen(buf), NULL);
    sock_udp_close(&sock);
}

int main(void) {
    while (1) {
        send_state();
        xtimer_usleep(INTERVAL_MS);
    }
    return 0;
}
