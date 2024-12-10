#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"
#define SEND_INTERVAL (5U * US_PER_SEC)

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP_TRUNC) {
        puts("Warning: remote server sent more data than the local buffer size");
    }
    else if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received\n");
    }
    else {
        printf("Request failed\n");
    }
}

int main(void)
{
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t server = {
        .family = AF_INET6,
        .port = SERVER_PORT
    };
    inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv6);

    while (1) {
        size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
        coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
        memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
        len += strlen(MESSAGE);
        gcoap_req_send(buf, len, &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        printf("Sent CoAP POST request to %s:%d with message: %s\n", SERVER_ADDR, SERVER_PORT, MESSAGE);

        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    return 0;
}