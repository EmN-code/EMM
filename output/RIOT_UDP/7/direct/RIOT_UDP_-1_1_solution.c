#include <xtimer.h>
#include <net/af.h>
#include <net/conn/udp.h>
#include <stdio.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    printf("UDP client example\n");

    kernel_pid_t pid = thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, NULL, "udp_sender");
    (void)pid;

    while (1) {
        char msg[] = "work";  // Change to "not work" when required
        sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
        ipv6_addr_from_str((ipv6_addr_t *)&server.addr, SERVER_ADDR);

        sock_udp_t sock;
        if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
            printf("Error creating UDP sock\n");
            return 1;
        }

        if (sock_udp_send(&sock, msg, strlen(msg), &server) < 0) {
            printf("Error sending UDP packet\n");
        } else {
            printf("Packet sent\n");
        }

        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}