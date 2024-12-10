#include "xtimer.h"
#include "thread.h"
#include "net/udp.h"
#include "net/sock/udp.h"

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

void *udp_send_loop(void *arg) {
    (void)arg;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Failed to create UDP sock\n");
        return NULL;
    }

    while (1) {
        char state[] = "work";
        if (sock_udp_send(&sock, state, sizeof(state), &remote) < 0) {
            printf("Error sending data\n");
        }
        xtimer_sleep(SEND_INTERVAL);
    }

    /* Close socket, though in practice we never get here */
    sock_udp_close(&sock);

    return NULL;
}

int main(void) {
    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, udp_send_loop, NULL, "udp_send_loop");
    return 0;
}
