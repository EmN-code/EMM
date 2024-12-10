#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "sock.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static coap_pkt_t pdu;

static const uint8_t message[] = "Hello, COAP Cloud";

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP_FAILURE) {
        puts("Failed to receive CoAP response");
        return;
    }
    puts("Received CoAP response");
}

int main(void) {
    sock_udp_ep_t remote;

    remote.family = AF_INET6;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET6, SERVER_IP, &(remote.addr.ipv6));

    unsigned msglen = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, message, sizeof(message));
    msglen += sizeof(message);

    ssize_t bytes_sent = gcoap_req_send(buf, msglen, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Error sending CoAP request: %d\n", (int)bytes_sent);
    } else {
        printf("CoAP request sent successfully, bytes: %d\n", (int)bytes_sent);
    }

    return 0;
}