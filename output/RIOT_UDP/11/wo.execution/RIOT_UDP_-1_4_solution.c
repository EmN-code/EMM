#include <mutex.h>
#include <netinet/in.h> 
#include <dht_params.h>
#include <net/emcute.h>
#include <arpa/inet.h>
#include <esp_system.h>
#include <net/sock/udp.h>
#include <sys/socket.h>
#include <net/ipv6/addr.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <stdio.h>
#include <ztimer.h>

#define SERVER_IP    "47.102.103.1"
#define SERVER_PORT  20001

int main(void) {
    sock_udp_ep_t server = { .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    sock_udp_t sock;
    sock_udp_ep_t *local = NULL; // NULL means automatically bind to available port
    if (sock_udp_create(&sock, local, NULL, 0) < 0) {
        puts("Error: Unable to create UDP socket");
        return -1;
    }

    const char *state = "work";
    size_t state_len = strlen(state);

    while (1) {
        if (sock_udp_send(&sock, state, state_len, &server) < 0) {
            puts("Error: Failed to send UDP packet");
        } else {
            puts("UDP packet sent");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
