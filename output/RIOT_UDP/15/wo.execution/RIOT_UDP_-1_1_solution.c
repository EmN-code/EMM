#include <net/af.h>
#include <net/conn_udp.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5 * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .netmask_len = 0,
                             .port = SERVER_PORT };
    if (ipv6_addr_from_str((ipv6_addr_t *)server.addr.ipv6, SERVER_IP) == NULL) {
        printf("Error setting server address\n");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    char *device_state = "work";  // Change to "not work" as necessary
    while (1) {
        int res = sock_udp_send(&sock, device_state, strlen(device_state), &server);
        if (res < 0) {
            printf("Error sending UDP packet\n");
            return 1;
        }
        printf("Sent state: %s\n", device_state);

        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}