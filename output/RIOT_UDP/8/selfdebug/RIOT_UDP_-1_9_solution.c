#include <stdio.h>  // Standard input-output library for printf()
#include <string.h>  // String operations like strncpy
#include "net/sock/udp.h"  // RIOT network API for UDP sockets
#include "xtimer.h"  // RIOT timer API for periodic execution
#include "net/af.h"  // Address family definitions
#include "kernel_defines.h"  // RIOT-specific kernel definitions

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define LOCAL_PORT 20002
#define SEND_INTERVAL (5 * 1000000) // 5 seconds in microseconds

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    char *message = "work";
    size_t len;

    /* Setup local endpoint */
    local.port = LOCAL_PORT;

    /* Setup remote endpoint */
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    
    /* Initialize the buffer for the IP address conversion */
    uint8_t addr_buffer[16];   // 16 bytes for IPv6 compatibility
    inet_pton(AF_INET, SERVER_IP, &addr_buffer);
    
    memcpy(&remote.addr, &addr_buffer, sizeof(remote.addr));  

    /* Create UDP socket */
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        printf("Error: Unable to create socket\n");
        return 1;
    }

    while (1) {
        len = strlen(message);
        int res = sock_udp_send(&sock, message, len, &remote);
        if (res < 0) {
            printf("Error sending message\n");
        } else {
            printf("Sent %s\n", message);
        }
        xtimer_usleep(SEND_INTERVAL);
    }
    return 0;
}