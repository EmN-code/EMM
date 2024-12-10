#include <sys/xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5U

void send_device_state(sock_udp_t *sock, sock_udp_ep_t *remote) {
    const char *state = "work";  // Change to "not work" as needed
    sock_udp_send(sock, state, strlen(state), remote);
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t server = {.family = AF_INET6, .port = SERVER_PORT};

    // Convert server IP address
    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse server address");
        return 1;
    }

    // Bind socket for UDP IPv6 communication
    if (sock_udp_create(&sock, &server, NULL, 0) < 0) {
        puts("Error: unable to create UDP sock");
        return 1;
    }

    while (1) {
        send_device_state(&sock, &server);
        xtimer_sleep(INTERVAL);
    }

    return 0;
}