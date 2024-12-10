#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "Device state: work or not work"
#define INTERVAL 5

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    struct sockaddr_in6 local_addr;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        // Handle IP conversion error
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, &local_addr, NULL, 0) < 0) {
        // Handle socket creation error
        return -1;
    }

    while (1) {
        ssize_t res = sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &remote);
        if (res < 0) {
            // Handle send error
        }
        // Sleep for defined interval
        xtimer_sleep(INTERVAL);
    }

    return 0;
}