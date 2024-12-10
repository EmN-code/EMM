#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)  // Send interval of 5 seconds.

int main(void) {
    sock_udp_ep_t remote;
    sock_udp_t sock;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    char SERVER_IP[] = "47.102.103.1";
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse IPv4 address");
        return 1;
    }

    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        const char *device_state = "work";  // Replace "work" with actual device state determination logic

        if (sock_udp_send(&sock, device_state, strlen(device_state), &remote) < 0) {
            puts("Error: unable to send UDP packet");
        } else {
            puts("Device state sent successfully");
        }

        xtimer_sleep(SEND_INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}