#include <thread.h>
#include <event/thread.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL (5U) // Interval in seconds

static char udp_stack[THREAD_STACKSIZE_DEFAULT];

void *udp_sender(void *arg) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR);
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &remote, 0);

    while (1) {
        sock_udp_send(&sock, DEVICE_STATE, strlen(DEVICE_STATE), &remote);
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    thread_create(udp_stack, sizeof(udp_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, udp_sender, NULL, "udp_sender");
    return 0;
}