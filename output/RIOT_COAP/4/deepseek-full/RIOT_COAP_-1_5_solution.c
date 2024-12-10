
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <arpa/inet.h>
#include <timex.h>
#include <msg.h>
#include <thread.h>
#include <kernel_defines.h>
#include <net/ipv6/addr.h>
#include <mutex.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static sock_udp_t sock;
static sock_udp_ep_t server_ep = SOCK_IPV4_EP_ANY;

static void _resp_handler(void *arg, sock_udp_ep_t *remote, void *data, size_t len)
{
    (void)arg;
    (void)remote;
    (void)data;
    (void)len;
    printf("Response received\n");
}

int main(void)
{
    sock_udp_ep_t local_ep = SOCK_IPV4_EP_ANY;
    local_ep.port = 12345;

    server_ep.port = SERVER_PORT;
    server_ep.family = AF_INET;

    if (inet_pton(AF_INET, SERVER_IP, &server_ep.addr.ipv4) <= 0) {
        printf("Invalid server IP address\n");
        return 1;
    }

    if (sock_udp_create(&sock, &local_ep, &server_ep, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    uint8_t buf[128];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    len += strlen(MESSAGE);

    if (sock_udp_send(&sock, buf, len, &server_ep) < 0) {
        printf("Failed to send CoAP request\n");
        sock_udp_close(&sock);
        return 1;
    }

    sock_udp_ep_t remote;
    uint8_t resp_buf[128];
    if (sock_udp_recv(&sock, resp_buf, sizeof(resp_buf), SOCK_NO_TIMEOUT, &remote) < 0) {
        printf("Failed to receive CoAP response\n");
        sock_udp_close(&sock);
        return 1;
    }

    _resp_handler(NULL, &remote, resp_buf, sizeof(resp_buf));

    sock_udp_close(&sock);
    return 0;
}
