#include <stdio.h>
#include <net/gcoap.h>
#include <net/ipv6/addr.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_METHOD COAP_METHOD_POST

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    printf("Response received from server: %.*s\n", pdu->payload_len, (char *)pdu->payload);
}

int main(void) {
    // Initialize buffer and CoAP packet
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Define server endpoint
    sock_udp_ep_t remote = { .family = AF_INET6, .port = COAP_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_IP);

    // Initialize CoAP request packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD, COAP_URI_PATH);

    // Set payload
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    // Send CoAP request
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL);

    return 0;
}