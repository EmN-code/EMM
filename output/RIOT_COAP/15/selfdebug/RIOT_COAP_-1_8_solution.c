#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/ipv6/addr.h>
#include <stdint.h>
#include <stdio.h>

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_UNUSED) {
        puts("timeout, no response received");
    }
    else {
        printf("Response received: %u\n", gcoap_get_code_class(pdu) << 5 | gcoap_get_code_detail(pdu));
    }
}

int main(void) {
    sock_udp_ep_t remote;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Convert IPv4 address string to byte representation
    if (inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse destination address");
        return 1;
    }
    remote.port = 5683;

    // Initialize CoAP PDU
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test") == -1) {
        puts("Failed to initialize CoAP request");
        return 1;
    }
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    pdu.payload_len = strlen("Hello, COAP Cloud");

    // Send CoAP request
    ssize_t res = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        puts("Error sending CoAP request");
        return 1;
    }
    puts("CoAP request sent");
    
    return 0;
}