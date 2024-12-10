
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_STATE_MSG_LEN 4
#define INTERVAL_SEC 5

static void send_device_state(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    sock_udp_t sock;
    char buf[DEVICE_STATE_MSG_LEN];

    if (inet_pton(AF_INET, UDP_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return;
    }

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: failed to create UDP socket");
        return;
    }

    strncpy(buf, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN);

    if (sock_udp_send(&sock, buf, DEVICE_STATE_MSG_LEN, NULL) < 0) {
        puts("Error: failed to send UDP message");
    } else {
        printf("Sent device state: %s\n", buf);
    }

    sock_udp_close(&sock);
}

int main(void)
{
    puts("RIOT UDP client example");

    while (1) {
        send_device_state();
        xtimer_sleep(INTERVAL_SEC);
    }

    return 0;
}
