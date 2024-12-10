#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP_TRUNC) {
        puts("Error: message truncated");
    }
    else if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Error: timeout");
    }
    else {
        printf("Response received with code: %u\n", coap_get_code(pdu));
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };

    /* Parse server IP address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP) == NULL) {
        puts("Error: unable to parse server address");
        return -1;
    }

    /* Init CoAP request */
    ssize_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL);
    printf("CoAP packet sent.\n");

    return 0;
}