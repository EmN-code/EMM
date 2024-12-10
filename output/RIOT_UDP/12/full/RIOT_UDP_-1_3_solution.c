#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "net/af.h"
#include "event/thread.h"
#include "sys/socket.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define LOCAL_PORT 0
#define DEVICE_STATE "work"

int main(void) {
    sock_udp_t sock;
    int rc;
    char *message = DEVICE_STATE;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = LOCAL_PORT;
    sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;
    sock_udp_ep_from_ipv6(&remote, SERVER_PORT, strlen(SERVER_IP), SERVER_IP);

    // Socket creation and binding in a loop until successful
    while (1) {
        if ((rc = sock_udp_create(&sock, &local, NULL, 0)) < 0) {
            printf("Error creating socket: %d\n", rc);
            continue;
        }
        else {
            printf("Socket created successfully\n");
            break;
        }
    }

    while (1) {
        // Send message to remote
        rc = sock_udp_send(&sock, message, strlen(message), &remote);

        if (rc < 0) {
            printf("Error sending UDP packet: %d\n", rc);
        } else {
            printf("Sent %d bytes to %s\n", rc, SERVER_IP);
        }

        // Wait for 5 seconds before sending the next packet
        sleep(5);
    }

    // Never reached, but included for completeness
    sock_udp_close(&sock);
    return 0;
}