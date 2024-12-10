#include <stdio.h>
#include <string.h>
#include <net/nanocoap.h>
#include <net/gcoap.h>
#include <net/ipv6/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received.\n");
    }
    else {
        printf("Request not sent successfully.\n");
    }
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6 };
    coap_pkt_t pdu;
    ssize_t bytes;
    
    uint8_t *pkt = buf;
    size_t len = CONFIG_GCOAP_PDU_BUF_SIZE;

    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    gcoap_req_init(&pdu, pkt, len, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    bytes = gcoap_req_send(pkt, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes <= 0) {
        printf("Failed to send request\n");
    }

    return 0;
}