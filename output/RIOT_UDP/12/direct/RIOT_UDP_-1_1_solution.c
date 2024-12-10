#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5U * 1000000U) // interval in microseconds

static void send_device_state(sock_udp_ep_t *remote) {
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }
    
    size_t len = strlen(DEVICE_STATE);
    if (sock_udp_send(&sock, DEVICE_STATE, len, remote) < 0) {
        puts("Error sending UDP packet");
    } else {
        puts("Device state sent successfully");
    }
    
    sock_udp_close(&sock);
}

int main(void) {
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    
    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_ADDR) == NULL) {
        puts("Error parsing server address");
        return 1;
    }
    
    while (1) {
        send_device_state(&server);
        xtimer_usleep(SEND_INTERVAL);
    }
    
    return 0;
}