#include <xtimer.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U)

static msg_t _main_msg_queue[8];

int main(void) {
    msg_init_queue(_main_msg_queue, 8);

    sock_udp_t sock;
    sock_udp_ep_t server = { .port = SERVER_PORT };
    ipv4_addr_t server_addr;

    if (ipv4_addr_from_str(&server_addr, SERVER_IP) == NULL) {
        puts("Error: unable to parse server IP");
        return 1;
    }
    server.family = AF_INET;
    memcpy(&server.addr.ipv4, &server_addr, sizeof(ipv4_addr_t));

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: cannot create UDP sock");
        return 1;
    }

    while (1) {
        const char *state = "work";  // or "not work"
        if (sock_udp_send(&sock, state, strlen(state), &server) < 0) {
            puts("Error: unable to send message");
        }
        else {
            puts("State sent successfully");
        }

        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}