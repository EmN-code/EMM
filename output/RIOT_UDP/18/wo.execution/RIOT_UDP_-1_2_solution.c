#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "ltc_ecc.h" // Corrected

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define LOCAL_PORT 12345 // Arbitrary port number for local

void send_device_state(sock_udp_t *sock, const sock_udp_ep_t *server) {
    const char *state = "work";
    ssize_t res = sock_udp_send(sock, state, strlen(state), server);
    if (res < 0) {
        printf("Error sending data: %d\n", (int)res);
    } else {
        printf("Data sent successfully\n");
    }
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };

    if (ipv4_addr_from_str((ipv4_addr_t *)&server.addr.ipv4, SERVER_IP) == NULL) {
        printf("Error: unable to parse server IP address\n");
        return 1;
    }

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }

    while (1) {
        send_device_state(&sock, &server);
        sleep(5);
    }

    return 0;
}