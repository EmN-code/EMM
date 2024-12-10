#include <stdio.h>
#include <net/sock/udp.h>
#include <net/inet.h>
#include <xtimer.h>

#define SERVER_PORT 20001
#define SERVER_ADDR "47.102.103.1"
#define SLEEP_INTERVAL (5U * US_PER_SEC) // Sleep for 5 seconds

static const char *state_msg = "work";

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[64];

    // Convert and assign server address
    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr) != 1) {
        printf("Error: unable to parse server address\n");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }

    while (1) {
        size_t msg_len = snprintf((char*)buf, sizeof(buf), "%s", state_msg);
        if (sock_udp_send(&sock, buf, msg_len, &remote) < 0) {
            printf("Error: unable to send message\n");
        } else {
            printf("Sent: %s\n", buf);
        }

        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    return 0;
}