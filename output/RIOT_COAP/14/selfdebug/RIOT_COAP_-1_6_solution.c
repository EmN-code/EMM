#include "net/sock/udp.h"
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

gcoap_req_send(const uint8_t *buf, size_t len, const sock_udp_ep_t *remote,
               gcoap_resp_handler_t resp_handler, void *context,
               gcoap_socket_type_t tl_type);

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response from server
    printf("Response received from server\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;

    remote.port = SERVER_PORT;
    int res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        printf("Error: Invalid IP address\n");
        return -1;
    }

    // Initialize CoAP request
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    pdu.payload_len = strlen(MESSAGE);

    // Send CoAP request
    gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    printf("CoAP request sent to %s:%d\n", SERVER_IP, SERVER_PORT);

    return 0;
}
