#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xtimer.h>
#include "net/sock/udp.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define MESSAGE_INTERVAL (5U * 1000U * 1000U)  // 5 seconds

static void publish_state(sock_udp_t *sock, sock_udp_ep_t *remote) {
    const char *state_work = "work";
    const char *state_not_work = "not work";
    char message[64];
    ssize_t res;

    memset(message, 0, sizeof(message));

    /* Toggle state between work and not work */
    static bool work_state = true;
    const char *state = work_state ? state_work : state_not_work;
    snprintf(message, sizeof(message), "state: %s", state);
    work_state = !work_state;

    res = sock_udp_send(sock, message, strlen(message), remote);
    if (res < 0) {
        printf("Error sending message: %d\n", res);
    }
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    int res;

    /* Parse the server address */
    res = inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
    if (res == 0) {
        printf("Error parsing server address\n");
        return 1;
    }

    /* Open a UDP socket */
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    /* Main loop */
    while (1) {
        publish_state(&sock, &remote);
        xtimer_sleep(MESSAGE_INTERVAL);
    }

    /* This code should ideally never be reached */
    sock_udp_close(&sock);
    return 0;
}