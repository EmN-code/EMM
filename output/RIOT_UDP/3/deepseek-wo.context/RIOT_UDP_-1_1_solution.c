
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
    inet_addr_from_str(UDP_SERVER_IP, &remote.addr.ipv4);

    int sock = sock_udp_create(&remote, NULL, 0);
    if (sock < 0) {
        puts("Failed to create UDP socket");
        return;
    }

    ssize_t res = sock_udp_send(sock, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN, &remote);
    if (res < 0) {
        puts("Failed to send UDP message");
    } else {
        printf("Sent device state: %s\n", DEVICE_STATE_MSG);
    }

    sock_udp_close(sock);
}

int main(void)
{
    puts("RIOT UDP Client for ESP32");

    while (1) {
        send_device_state();
        xtimer_sleep(INTERVAL_SECONDS);
    }

    return 0;
}
