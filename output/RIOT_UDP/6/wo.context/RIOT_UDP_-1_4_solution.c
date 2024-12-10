#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_PORT 20001
#define SERVER_IP "47.102.103.1"
#define MESSAGE "work"
#define NOT_WORK_MESSAGE "not work"
#define INTERVAL (5U * US_PER_SEC)

static char message[] = MESSAGE;

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT};
    sock_udp_t sock;
    int res;

    /* Create UDP socket */
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    /* Convert IP address from text to binary form */
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        puts("inet_pton failed");
        return 1;
    }

    xtimer_ticks32_t last = xtimer_now();

    while (1) {
        /* Send the message */
        res = sock_udp_send(&sock, message, sizeof(message), &server);
        if (res < 0) {
            printf("Error sending UDP packet: %d\n", res);
        } else {
            puts("Message sent");
        }

        /* Simulate a change in state for demonstration */
        if (strcmp(message, MESSAGE) == 0) {
            strcpy(message, NOT_WORK_MESSAGE);
        } else {
            strcpy(message, MESSAGE);
        }

        xtimer_periodic_wakeup(&last, INTERVAL);
    }

    /* Close the socket */
    sock_udp_close(&sock);
    return 0;
}
