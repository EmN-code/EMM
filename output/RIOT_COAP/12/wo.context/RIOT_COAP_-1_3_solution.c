#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    
    if (pdu != NULL) {
        printf("Response received: %s\n", (char *)pdu->payload);
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    // CoAP RFC7641 constants
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t res;

    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    const char *payload = "Hello, CoAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    sock_udp_ep_t remote = { .family = AF_INET6, .port = 5683 };
    inet_pton(AF_INET6, "47.102.103.1", &remote.addr.ipv6);

    // Send CoAP request
    res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending CoAP request\n");
    }
    
    return 0;
}