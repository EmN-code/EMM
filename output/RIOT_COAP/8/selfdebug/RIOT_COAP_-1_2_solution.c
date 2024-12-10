#include "net/gcoap.h"
#include "net/af.h"
#include "net/gcoap_sock.h"
#include "net/ipv6/addr.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/pktdump.h"
#include "net/gnrc/udp.h"
#include "net/gnrc.h"
#include "kernel.h"
#include <stdio.h>

#define SERVER_PORT 5683
#define SERVER_IP "47.102.103.1"
#define COAP_BUFFER_SIZE 128
#define URI_PATH "/coap/test"

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void) {
    uint8_t buf[COAP_BUFFER_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    size_t len = gcoap_req_init(&pdu, buf, COAP_BUFFER_SIZE, COAP_METHOD_POST, URI_PATH);
    ssize_t payload_len = snprintf((char *)pdu.payload, COAP_BUFFER_SIZE - len, "Hello, COAP Cloud");
    if (payload_len < 0) {
        printf("Error creating payload\n");
        return -1;
    }

    len += payload_len;

    if (gcoap_req_send(buf, len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESPONSE) {
        printf("Response received\n");
    } else {
        printf("Failed to receive response\n");
    }
}