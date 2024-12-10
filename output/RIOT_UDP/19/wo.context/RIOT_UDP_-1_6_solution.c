#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/protnum.h>
#include <net/ipv4/addr.h> 
#include <net/af.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

static const char *device_state = "work";

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t server_addr[IPV4_ADDR_LEN];

    if (inet_pton(AF_INET, SERVER_IP, &server_addr) != 1) {
        puts("Error: unable to parse server address");
        return 1;
    }

    memcpy(&remote.addr, server_addr, sizeof(server_addr));

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, device_state, strlen(device_state), &remote) < 0) {
            puts("Error: unable to send data");
        }
        else {
            printf("Sent: %s\n", device_state);
        }
        xtimer_sleep(5);
    }

    return 0;
}