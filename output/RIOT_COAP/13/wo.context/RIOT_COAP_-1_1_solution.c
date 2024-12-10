#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/af.h"
#include "net/sock/udp.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT  // COAP default port is 5683
#define URI_PATH "/coap/test"

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                              const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_FIN) {
        printf("Response received\n");
    } else {
        printf("Request timed out\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Initialize remote address and port
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (ipv6_addr_from_str((ipv6_addr_t *)remote.addr.ipv6, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse CoAP server address\n");
        return -1;
    }

    // Initialize CoAP packet
    int res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (res < 0) {
        printf("Error: unable to initialize CoAP request\n");
        return -1;
    }

    // Add payload to CoAP request
    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD) < 0) {
        printf("Error: unable to finish CoAP options\n");
        return -1;
    }
    memcpy(pdu.payload, "Hello, COAP Cloud", 18);
    pdu.payload_len = 18;

    // Send CoAP request
    res = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Error: unable to send CoAP request\n");
        return -1;
    }

    return 0;
}