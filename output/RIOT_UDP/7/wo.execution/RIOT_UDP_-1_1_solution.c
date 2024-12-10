#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <net/af.h>
#include <net/protnum.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds

static char stack[THREAD_STACKSIZE_DEFAULT];

static void *udp_sender(void *arg) {
    (void)arg; // Suppress unused parameter warning
    sock_udp_ep_t remote = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF, .port = SERVER_PORT };
    sock_udp_t sock;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Could not create UDP socket");
        return NULL;
    }

    while (1) {
        const char *message = "work"; // Example device state message
        if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
            puts("Error sending message");
        } else {
            printf("Sent: %s\n", message);
        }
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC); // Sleep for defined interval
    }
    return NULL;
}

int main(void) {
    puts("UDP client started\n");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, udp_sender, NULL, "udp_sender");
    return 0;
}