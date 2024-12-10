#include <string.h>
#include <stdio.h>
#include "xtimer.h"
#include "net/gnrc/netif.h"
#include "net/af.h"
#include "net/protnet/protnet.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT "20001"
#define SLEEP_INTERVAL (5 * US_PER_SEC)

int main(void) {
    gnrc_netif_t *netif = NULL;
    int retry = 0;
    
    while (netif == NULL) {
        netif = gnrc_netif_iter(NULL);
        if (netif == NULL) {
            printf("Waiting for network interface...");
            xtimer_sleep(1);
        }
    }

    char message[12];
    int work_status = 0;

    uint8_t remote_addr[16];
    size_t remote_len = sizeof(remote_addr);

    if (netopt_str2addr(remote_addr, &remote_len, SERVER_ADDR) < 0) {
        printf("Invalid Server Address\n");
        return -1;
    }

    sock_udp_ep_t remote = {.family = AF_INET6, .port = atoi(SERVER_PORT), .addr = {0}};
    memcpy(remote.addr.ipv6, remote_addr, remote_len);

    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_ADDR_ANY_EP;
    local.port = 0;

    while (1) {
        if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
            printf("Error creating UDP socket\n");
            xtimer_sleep(1);
            continue;
        }
        break;
    }

    while (1) {
        sprintf(message, "Work: %d", work_status);
        if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
            printf("Error sending packet\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Sent message: %s\n", message);
        }
        work_status = 1 - work_status;
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    sock_udp_close(&sock);
    return 0;
}