#include <stdio.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, 
                          const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_RESP){
        if (pdu->code == COAP_CODE_204) {
            puts("Success: received 2.04 Changed response");
        }
    }
    else if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Error: request timed out");
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = CONFIG_GCOAP_PORT };

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result == 0) {
        puts("Error: malformed IP address");
        return 1;
    }
    else if (result < 0) {
        puts("Error: AF not supported");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    ssize_t bytes_sent = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        puts("Error: failed to send request");
    }
    else {
        puts("CoAP request sent");
    }

    return 0;
}