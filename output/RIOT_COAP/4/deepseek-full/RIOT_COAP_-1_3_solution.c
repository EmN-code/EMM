
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/sock/util.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mma8x5x_regs.h>
#include <time_units.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(void *arg, sock_udp_ep_t *remote, sock_udp_sock_t *sock, sock_udp_ep_t *local)
{
    (void)arg;
    (void)remote;
    (void)sock;
    (void)local;
    printf("CoAP response received\n");
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid address\n");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    uint8_t buf[128];
    size_t len = snprintf((char *)buf, sizeof(buf), "%s", MESSAGE);

    if (sock_udp_send(&sock, buf, len, &remote) < 0) {
        printf("Failed to send CoAP request\n");
        sock_udp_close(&sock);
        return 1;
    }

    msg_t msg;
    if (sock_udp_recv(&sock, &msg, sizeof(msg), SOCK_NO_TIMEOUT, NULL) < 0) {
        printf("Failed to receive CoAP response\n");
    }

    sock_udp_close(&sock);
    return 0;
}
