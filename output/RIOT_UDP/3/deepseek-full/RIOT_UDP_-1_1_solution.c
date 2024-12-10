
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_STATE_MSG_LEN 4
#define INTERVAL_SECONDS 5

static void send_device_state(void) {
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
        puts("Device state sent");
    }

    sock_udp_close(&sock);
}

int main(void) {
    while (1) {
        send_device_state();
        xtimer_sleep(INTERVAL_SECONDS);
    }

    return 0;
}
