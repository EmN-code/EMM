#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "net/af.h"
#include "msg.h"

#define SERVER_IP "47.102.103.1"
#define PORT 1883
#define INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds
#define MAX_PAYLOAD_LEN 128

static void publish_state(const char *state) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = PORT };
    inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &remote, 0);

    char payload[MAX_PAYLOAD_LEN];
    snprintf(payload, sizeof(payload), "{\"state\": \"%s\"}", state);

    sock_udp_send(&sock, payload, strlen(payload), &remote);

    sock_udp_close(&sock);
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (1) {
        publish_state("work");
        printf("Published state: work\n");
        xtimer_usleep(INTERVAL);

        publish_state("not work");
        printf("Published state: not work\n");
        xtimer_usleep(INTERVAL);
    }

    return 0;
}