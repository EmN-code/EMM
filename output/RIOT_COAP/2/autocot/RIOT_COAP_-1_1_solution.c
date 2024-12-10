#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu->payload_len)
        printf("Response received: %.*s\n", pdu->payload_len, (char *)pdu->payload);
    else
        printf("No payload received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len;
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };

    // Parse server IP
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);

    // Initialize CoAP packet
    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_uint(&pdu, COAP_OPT_CONTENT_FORMAT, COAP_FORMAT_TEXT);
    len += coap_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    // Send CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0)
        printf("Error sending CoAP request\n");
    else
        printf("CoAP POST request sent\n");

    return 0;
}