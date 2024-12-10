#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu) {
        printf("Response received: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
    }
    else {
        printf("No response\n");
    }
}

int main(void) {
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    size_t len = strlen(PAYLOAD);
    if (len > gcoap_get_payload(pdu, &pdu)) {
        printf("Error: payload too large\n");
        return -1;
    }

    memcpy(pdu.payload, PAYLOAD, len);
    gcoap_finish(&pdu, len, COAP_FORMAT_TEXT);

    int result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error converting IP address\n");
        return -1;
    }

    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}