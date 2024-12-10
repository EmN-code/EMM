#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define SERVER_IP       "47.102.103.1"
#define SERVER_PORT     20001
#define SEND_INTERVAL   (5U * US_PER_SEC) // send data every 5 seconds

static const char *device_state = "work";

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    uint8_t addr[16];  
    inet_pton(AF_INET6, SERVER_IP, addr);
    memcpy(&remote.addr, addr, sizeof(addr));
    
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return -1;
    }

    while (1) {
        ssize_t res;
        res = sock_udp_send(&sock, device_state, strlen(device_state), &remote);
        if (res < 0) {
            puts("Error sending UDP packet");
        } else {
            puts("Device state sent");
        }
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}