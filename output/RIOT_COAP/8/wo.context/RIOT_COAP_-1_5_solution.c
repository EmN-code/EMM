#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/ipv6/addr.h>
#include <string.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu != NULL) {
        printf("Success: Received response with code %u\n", coap_get_code(pdu));
    } else {
        printf("Error: Timeout or error occurred\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    coap_pkt_t pdu;

    /* set remote address */
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    /* initialize CoAP request */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    size_t len = pdu.hdr_len + pdu.payload_len;

    /* send the CoAP request */
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    
    return 0;
}