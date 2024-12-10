
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_STATE_MSG_LEN 4
#define INTERVAL_SECONDS 5

static void send_device_state(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    inet_pton(AF_INET, UDP_SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &remote, 0);

    while (1) {
        int res = sock_udp_send(&sock, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN, NULL);
        if (res < 0) {
            printf("Failed to send UDP message\n");
        } else {
            printf("Sent device state: %s\n", DEVICE_STATE_MSG);
        }

        xtimer_sleep(INTERVAL_SECONDS);
    }

    sock_udp_close(&sock);
}

int main(void) {
    puts("Starting UDP client...");
    send_device_state();
    return 0;
}
