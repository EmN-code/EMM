#include <stdio.h>
#include <string.h>
#include "kernel/thread.h"
#include "kernel/timer.h"
#include "net/sock/udp.h"

#define UDP_SERVER "47.102.103.1"
#define UDP_PORT 20001
#define MESSAGE "work"

static char stack[THREAD_STACKSIZE_MAIN];

void *udp_send_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t remote = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF, .port = UDP_PORT };
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, UDP_SERVER) == NULL) {
        puts("Error: unable to parse server address");
        return NULL;
    }

    sock_udp_t sock;
    int res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        puts("Error: unable to create UDP socket");
        return NULL;
    }

    while (1) {
        ssize_t sent_bytes = sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &remote);
        if (sent_bytes < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent successfully");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    puts("UDP client example");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, udp_send_thread, NULL, "udp_send");
    return 0;
}