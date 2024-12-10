#include <string.h>
#include <stdint.h>
#include <net/ipv6/addr.h>

#define COAP_PORT 5683
#define COAP_SERVER "47.102.103.1"
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

gcoap_request_memo_t memo;
sock_udp_ep_t server_ep;

void send_coap_request(void) {
    coap_pkt_t pdu;
    size_t len;

    server_ep.family = AF_INET6;
    server_ep.netif  = SOCK_ADDR_ANY_NETIF;
    server_ep.port   = COAP_PORT;

    inet_pton(AF_INET, COAP_SERVER, &server_ep.addr.ipv6);

    /* Initialize CoAP request with POST method */
    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    /* Set payload for POST request */
    memcpy(pdu.payload, COAP_MESSAGE, sizeof(COAP_MESSAGE));
    pdu.payload_len = sizeof(COAP_MESSAGE);

    /* Send the gcoap request */
    if (gcoap_req_send(buf, len, &server_ep, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error sending request");
    } else {
        puts("Request sent");
    }
}

int main(void) {
    send_coap_request();
    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    puts("Response received");
}