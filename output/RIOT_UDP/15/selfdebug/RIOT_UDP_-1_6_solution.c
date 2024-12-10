#include <net/sock/udp.h>
#include <xtimer.h>
#include <string.h>

#define UDP_SERVER "47.102.103.1"
#define UDP_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t remote = {.family = AF_INET, .port = UDP_PORT};

    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, UDP_SERVER) == NULL) {
        printf("Error: unable to parse address\n");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return -1;
    }

    char message[] = "Device State: work";
    while (1) {
        if (sock_udp_send(&sock, message, sizeof(message), &remote) < 0) {
            printf("Error: unable to send message\n");
        } else {
            printf("Message sent: %s\n", message);
        }
        xtimer_sleep(INTERVAL);
    }
    return 0;
}