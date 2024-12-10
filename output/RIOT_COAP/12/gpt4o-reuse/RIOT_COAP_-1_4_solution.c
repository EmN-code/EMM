
#include <stdio.h>
#include <netinet/in.h>
#include "gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    if (pdu->hdr->code == COAP_CODE_CONTENT) {
        uint8_t *payload = coap_get_payload(pdu);
        size_t payload_len = coap_get_payload_len(pdu);
        printf("Received response: %.*s\n", (int)payload_len, payload);
    } else {
        printf("Error in response\n");
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_add_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 17);

    ssize_t len = gcoap_req_send(&pdu, coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len < 0) {
        printf("Failed to send CoAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }

    while (1) {
        gcoap_event_loop();
    }

    return 0;
}
