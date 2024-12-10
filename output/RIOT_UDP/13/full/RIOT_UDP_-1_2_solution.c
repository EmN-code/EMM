#include "udp.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include "net/af.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define LOCAL_PORT 12345
#define MESSAGE "work"
#define WAIT_TIME (5 * UINT32_C(1000000))

int main(void) {
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = LOCAL_PORT;

    sock_udp_ep_t remote;
    sock_udp_str2ep(&remote, SERVER_ADDR);
    remote.port = SERVER_PORT;

    sock_udp_t sock;
    while (1) {
        if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
            continue; // Retry on failure
        } else {
            break; // Successfully created socket
        }
    }

    while (1) {
        if (sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &remote) < 0) {
            continue; // Retry on failure
        }
        xtimer_usleep(WAIT_TIME); // Wait for 5 seconds
    }

    return 0;
}