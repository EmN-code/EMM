
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
    inet_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, UDP_SERVER_IP);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &remote, 0);

    while (1) {
        int res = sock_udp_send(&sock, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN, NULL);
        if (res < 0) {
            printf("Failed to send UDP message: %d\n", res);
        } else {
            printf("Sent device state: %s\n", DEVICE_STATE_MSG);
        }

        xtimer_usleep(INTERVAL_MS);
    }

    sock_udp_close(&sock);
}

int main(void)
{
    puts("Starting UDP client...");
    send_device_state();
    return 0;
}
