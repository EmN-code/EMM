#include "xtimer.h"
#include "net/af.h"
#include "net/protnet/udp.h"
#include "net/gnrc/ipv6/addr.h"
#include "net/gnrc/inet/ntoh.h"
#include "unistd.h"

int main(void) {
    // Initialize variables
    gnrc_netif_t *netif = gnrc_netif_iter(NULL);
    sock_udp_ep_t server = { .family = AF_INET, .netif = netif->pid };
    char *server_addr = "47.102.103.1";
    char message[] = "work";

    // Parse server address and port
    if (gnrc_ipv6_netmask_parse(&server.addr.ipv6, server_addr) < 0) {
        puts("Error: unable to parse server address");
        return 1;
    }
    server.port = 20001;
    
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET6;
    gnrc_ipv6_netmask_parse(&remote.addr.ipv6, server_addr);
    remote.port = atoi(server.port);
    
    // Create UDP sock
    sock_udp_t sock;
    int res = sock_udp_create(&sock, &remote, NULL, 0);
    if (res < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    while (1) {
        // Send message to the server
        ssize_t bytes_sent = sock_udp_send(&sock, message, sizeof(message), &server);
        if (bytes_sent < 0) {
            puts("Error sending message");
        }
        else {
            printf("Sent message: %s\n", message);
        }
        
        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}