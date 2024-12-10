#include "net/sock/util.h"
#include "thread.h"
#include "mutex.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "net/sock/udp.h"

#define RECV_PORT   (20001)
#define SERVER_ADDR "47.102.103.1"

int main(void) {
    /* Initialize variables */
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_ep_t remote;
    remote.family = AF_INET6;
    remote.port = RECV_PORT;
    inet_pton(AF_INET6, SERVER_ADDR, &remote.addr.ipv6);

    /* Create UDP socket */
    if (sock_udp_create(&sock, &local, &remote, 0) != 0) {
        puts("Error: Unable to create socket");
        return -1;
    }

    /* Data to send */
    const char *state = "work";

    /* Main loop */
    while (1) {
        ssize_t res = sock_udp_send(&sock, state, strlen(state), &remote);
        if (res < 0) {
            printf("Error sending packet: %d\n", (int)res);
            sock_udp_close(&sock); /* Close and attempt to recreate socket */
            if (sock_udp_create(&sock, &local, &remote, 0) != 0) {
                puts("Error: Unable to recreate socket");
                return -1;
            }
        } else {
            printf("Sent: %s\n", state);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000); /* Wait for 5 seconds */
    }

    /* Clean up */
    sock_udp_close(&sock);
    return 0;
}