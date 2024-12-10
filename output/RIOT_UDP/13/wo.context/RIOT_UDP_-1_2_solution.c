#include <xtimer.h>
#include <stdio.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_PORT (20001)
#define TIMER_INTERVAL (5U * US_PER_SEC)
#define SERVER_IP "47.102.103.1"

static void send_device_state(sock_udp_t *sock, sock_udp_ep_t *remote) {
    const char *msg = "work";
    printf("Sending device state: %s\n", msg);
    sock_udp_send(sock, msg, strlen(msg), remote);
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint32_t timer = TIMER_INTERVAL;

    // Convert IP address from string to binary
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr, SERVER_IP) == NULL) {
        puts("Error: unable to parse remote address");
        return 1;
    }

    // Open UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to initialize UDP socket");
        return 1;
    }

    // Timer to send UDP packets
    while (1) {
        send_device_state(&sock, &remote);
        xtimer_sleep(timer);
    }

    return 0;
}