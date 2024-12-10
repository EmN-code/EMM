#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <netinet/in.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

static char *device_state = "work"; // Device state, either "work" or "not work"

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    char addr_str[NETADDR_MAXLEN];
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        if (sock_udp_send(&sock, device_state, strlen(device_state), &remote) < 0) {
            printf("Error sending UDP message\n");
        } else {
            printf("Sent device state: %s\n", device_state);
        }
        
        xtimer_sleep(5);
    }

    return 0;
}