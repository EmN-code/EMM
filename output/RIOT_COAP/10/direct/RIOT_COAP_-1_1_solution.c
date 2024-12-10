#include <stdio.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "kernel.h"
#include "msg.h"

#define COAP_PORT "5683"
#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_URI_PATH "/coap/test"
#define COAP_MSG "Hello, COAP Cloud"
#define COAP_RESPONSE_WAIT 3

static ssize_t _response_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len)
{
    (void)buf;
    (void)len;

    if (pdu->hdr->code == COAP_CODE_204) {
        printf("Success: Resource changed\n");
    }
    else {
        printf("Error: %u\n", pdu->hdr->code);
    }

    return 0;
}

int main(void)
{
    gcoap_req_init_t params;
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    uint8_t token[2] = {0x9a, 0x36};

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = atoi(COAP_PORT);
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_ADDR);

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    memcpy(pdu.token, token, sizeof(token));
    pdu.tkl = sizeof(token);

    memcpy(pdu.payload, COAP_MSG, strlen(COAP_MSG));
    pdu.payload_len = strlen(COAP_MSG);

    ssize_t res = gcoap_req_send(buf, pdu.payload_len + pdu.payload - buf, &remote, _response_handler);

    if (res < 0) {
        printf("gcoap_req_send() failed with %zd\n", res);
    }
    else {
        printf("gcoap_req_send() success\n");
    }

    return 0;
}