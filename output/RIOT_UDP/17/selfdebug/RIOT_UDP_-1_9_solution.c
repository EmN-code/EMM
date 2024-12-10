#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define LOCAL_PORT 12345  // Can be any port number

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = LOCAL_PORT;

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    char state[32];
    int res = sock_udp_create(&sock, &local, &remote, 0);

    if (res < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    while (1) {
        snprintf(state, sizeof(state), "Device is %s", "work");  // Change this to "not work" as needed
        sock_udp_send(&sock, state, strlen(state), NULL);
        puts("State sent to server");
        xtimer_sleep(5);
    }

    // This part of code is unreachable but added for good practice.
    sock_udp_close(&sock);
    return 0;
}