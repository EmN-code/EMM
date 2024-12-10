#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_PORT CONFIG_GCOAP_PORT
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Timeout for msg ID %u\n", coap_get_id(pdu));
        return;
    }

    unsigned code = coap_get_code(pdu);
    if (code == COAP_CODE_205) {
        printf("Success: Received a response\n");
    }
    else {
        printf("Error: COAP code %u\n", code);
    }
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;

    /* Set remote address and port */
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.port = COAP_PORT;

    /* Initialize CoAP request */
    gcoap_req_init(&pdu, &buf[0], CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    ssize_t len = gcoap_finish(&pdu, strlen(PAYLOAD), COAP_FORMAT_TEXT);

    if (len < 0) {
        puts("Error creating CoAP request\n");
        return 1;
    }

    /* Send CoAP request */
    if (gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error sending CoAP request\n");
        return 1;
    }

    puts("CoAP request sent\n");

    return 0;
}