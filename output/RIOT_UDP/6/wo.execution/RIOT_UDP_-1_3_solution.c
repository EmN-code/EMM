#include "netinet/in.h"
#include "net/emcute.h"
#include "arpa/inet.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/gcoap.h"

#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001

int main(void) {
    
    sock_udp_ep_t remote = {};
    remote.family = AF_INET;
    remote.port = UDP_SERVER_PORT;

    if (inet_pton(AF_INET, UDP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error converting IP address\n");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return -1;
    }

    char message[] = "work"; // or "not work"

    while (1) {
        if (sock_udp_send(&sock, message, sizeof(message), &remote) < 0) {
            printf("Error sending message\n");
        } else {
            printf("Message sent: %s\n", message);
        }

        sleep(5);
    }

    return 0;
}