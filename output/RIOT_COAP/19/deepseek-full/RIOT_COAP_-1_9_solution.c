
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <stdbool.h>
#include <msg.h>
#include <sys/socket.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define BUF_SIZE 128

static void _resp_handler(void *arg, sock_udp_ep_t *remote, const uint8_t *buf, size_t len, sock_udp_t *sock)
{
    (void)arg;
    (void)remote;
    (void)buf;
    (void)len;
    (void)sock;
    printf("Response received\n");
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    sock_udp_t sock;
    uint8_t buf[BUF_SIZE];
    int res;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;

    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        printf("Invalid IP address\n");
        return -1;
    }

    res = sock_udp_create(&sock, &local, &remote, 0);
    if (res < 0) {
        printf("Failed to create socket\n");
        return -1;
    }

    while (1) {
        printf("Sending CoAP request...\n");
        res = sock_udp_send(&sock, PAYLOAD, strlen(PAYLOAD), &remote);
        if (res < 0) {
            printf("Failed to send request\n");
        } else {
            printf("Request sent\n");
        }

        xtimer_sleep(5);
    }

    sock_udp_close(&sock);
    return 0;
}
