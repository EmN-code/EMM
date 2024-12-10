#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>  // Import sock UDP header
#include <net/af.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    // Convert server IP address
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return -1;
    }

    char *state_msg = "work";

    while (1) {
        ssize_t res = sock_udp_send(&sock, state_msg, strlen(state_msg), &remote);
        if (res < 0) {
            printf("Error: unable to send UDP message\n");
        } else {
            printf("Successfully sent state message 'work'\n");
        }
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);  // Sleep for the send interval
    }

    return 0;
}