#include <stdio.h>
#include <string.h>
#include <net/gcoap.h>
#include <net/af.h>
#include <net/sock/udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define COAP_METHOD COAP_METHOD_POST
#define PAYLOAD "Hello, COAP Cloud"

static gcoap_request_memo_t *memo;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    printf("Response received from server with code %u\n", coap_get_code(pdu));
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t server_ep = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF };

    server_ep.port = SERVER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&server_ep.addr.ipv6, SERVER_ADDR);

    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD, URI_PATH) < 0) {
        printf("Failed to initialize CoAP Request\n");
        return -1;
    }

    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_add_payload_marker(&pdu);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    if (gcoap_req_send(buf, pdu.payload_len, &server_ep, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Failed to send CoAP Request\n");
        return -1;
    }

    printf("CoAP request sent, waiting for response...\n");

    return 0;
}
