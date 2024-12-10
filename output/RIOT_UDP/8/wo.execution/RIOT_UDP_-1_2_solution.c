#include <sys/socket.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>

#define UDP_SERVER_ADDR "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL_S 5

static void send_device_state(sock_udp_t *sock, sock_udp_ep_t *remote) {
    const char *device_state = "work";
    ssize_t res = sock_udp_send(sock, device_state, strlen(device_state), remote);
    if (res < 0) {
        printf("Error sending data: %d\n", res);
    }
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, UDP_SERVER_ADDR);

    if (sock_udp_create(&sock, &remote, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    while (1) {
        send_device_state(&sock, &remote);
        sleep(SEND_INTERVAL_S);
    }

    sock_udp_close(&sock);
    return 0;
}