#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define LOCAL_PORT 12345

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .ipv6 = IPV6_ADDR_ANY_INIT, .port = 0 }; 
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;  
    sock_udp_t sock;
    char state_buffer[15];
    int is_working = 1;

    inet_pton(AF_INET6, SERVER_ADDR, remote.ipv6);
    remote.port = SERVER_PORT;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    while (1) {
        snprintf(state_buffer, sizeof(state_buffer), "State: %s", is_working ? "work" : "not work");
        if (sock_udp_send(&sock, state_buffer, strlen(state_buffer), &remote) < 0) {
            printf("Failed to send UDP packet\n");
            return 1;
        }

        is_working = !is_working;

        xtimer_sleep(5);
    }

    return 0;
}