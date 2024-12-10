
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
    inet_addr_t addr;
    sock_udp_t sock;
    char buf[64];

    if (inet_pton(AF_INET, UDP_SERVER_IP, &addr) <= 0) {
        puts("Error converting IP address");
        return;
    }
    remote.addr.ipv4 = addr;

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    strncpy(buf, DEVICE_STATE_MSG, sizeof(buf));
    if (sock_udp_send(&sock, buf, strlen(buf), NULL) < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }

    sock_udp_close(&sock);
}

int main(void)
{
    puts("RIOT UDP Client for ESP32");

    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
