#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define EMCUTE_PORT (5683)
#define EMCUTE_DEFAULT_IP "47.102.103.1"
#define EMCUTE_URI_PATH "/coap/test"
#define GCOAP_SOCKET_TYPE_UDP (1)
#define PAYLOAD "Hello, COAP Cloud"
#define CONFIG_GCOAP_PDU_BUF_SIZE (128)
#define CONFIG_GCOAP_PORT (5683)

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    /* Handle response here */
    if (memo->state == GCOAP_RESPONSE) {
        printf("Response received\n");
        if (pdu->payload_len > 0) {
            printf("Payload: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
        }
    }
    else {
        printf("Error in response\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    inet_pton(AF_INET, EMCUTE_DEFAULT_IP, &remote.addr.ipv4);
    remote.port = CONFIG_GCOAP_PORT;

    /* Initialize CoAP PDU */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMCUTE_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    /* Send the CoAP request */
    if (gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending request\n");
    }
    else {
        printf("Request sent\n");
    }
    
    return 0;
}