#include "net/sock/udp.h"
#include "xtimer.h"
#include "msg.h"

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT    20001
#define INTERVAL       (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = {.family = AF_INET, .port = SERVER_PORT};
    sock_udp_t sock;
    msg_init_queue(msg_t_queue, ARRAY_SIZE(msg_t_queue));

    if (inet_pton(AF_INET, SERVER_ADDRESS, &server.addr.ipv4) != 1) {
        printf("Error: unable to parse IPv4 address\n");
        return -1;
    }

    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        printf("Error: unable to create UDP sock\n");
        return -1;
    }

    printf("UDP connection to %s:%d established\n", SERVER_ADDRESS, SERVER_PORT);

    while (1) {
        const char *state = "work";  // or "not work" depending on some condition
        if (sock_udp_send(&sock, state, strlen(state), &server) < 0) {
            printf("Error: unable to send data\n");
        } else {
            printf("Sent device state: %s\n", state);
        }
        xtimer_sleep(INTERVAL);
    }
    
    return 0;
}