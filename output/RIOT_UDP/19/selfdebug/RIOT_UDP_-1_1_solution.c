#include <thread.h>
#include <udp.h>
#include <xtimer.h>
#include <stdio.h>
#include <net/gnrc.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL 5U  // Send every 5 seconds
#define MESSAGE "work"

static char stack[THREAD_STACKSIZE_DEFAULT];

static void *_send_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };

    if (gnrc_netif_get_by_pid(THREAD_PID_FIRST) == NULL) {
        printf("Error: no network interface found\n");
        return NULL;
    }

    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse IPv6 address\n");
        return NULL;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return NULL;
    }

    while (1) {
        if (sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &remote) < 0) {
            printf("Error: unable to send UDP packet\n");
        }
        xtimer_sleep(SEND_INTERVAL);
    }

    return NULL;
}

int main(void) {
    printf("Starting UDP client\n");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, _send_thread, NULL, "send thread");
    return 0;
}