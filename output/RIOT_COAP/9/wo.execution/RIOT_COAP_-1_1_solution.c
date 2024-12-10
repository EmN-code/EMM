#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "net/gcoap.h"
#include "xtimer.h"

#define COAP_PORT 5683
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_URI_PATH "/coap/test"
#define COAP_METHOD COAP_METHOD_POST
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_STATE_RESP)
        printf("Success: received response\n");
    else
        printf("Failed to receive response\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    
    // Configure remote socket
    remote.family = AF_INET6;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_IP);

    // Initialize CoAP PDU
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD, COAP_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    memcpy(pdu.payload, MESSAGE, sizeof(MESSAGE)-1);
    coap_hdr_set_payload_len(pdu.hdr, sizeof(MESSAGE)-1);

    // Send CoAP request
    if (gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL) < 0) 
    {
        printf("Failed to send request\n");
    }

    return 0;
}