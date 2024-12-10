#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sock/udp.h>
#include <net/gnrc/netif.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U)  // 5 seconds interval

static char stack[THREAD_STACKSIZE_DEFAULT];
static kernel_pid_t network_if_pid;

void *udp_sender(void *arg) {
    (void)arg;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    char message[20];

    if (inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6) == 0) {
        puts("Error: unable to parse destination address");
        return NULL;
    }
    
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return NULL;
    }

    while (1) {
        snprintf(message, sizeof(message), "State: work");
        if (sock_udp_send(&sock, message, strlen(message), NULL) < 0) {
            printf("Error: unable to send message\n");
        } else {
            printf("Sent: %s\n", message);
        }

        xtimer_sleep(INTERVAL);
    }

    return NULL; // Not reached
}

int main(void) {
    puts("RIOT UDP sender example");

    gnrc_netif_t *netif = gnrc_netif_get(0);
    if (netif == NULL) {
        puts("Error: No network interface found");
        return 1;
    }

    network_if_pid = netif->pid;
    if (thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                      THREAD_CREATE_STACKTEST, udp_sender, NULL, "udp_sender") <= KERNEL_PID_UNDEF) {
        puts("Error: thread_create failed");
        return 1;
    }

    return 0;
}