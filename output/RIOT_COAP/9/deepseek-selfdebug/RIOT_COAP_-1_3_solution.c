
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else {
        printf("Response Code: %u\n", coap_get_code(pdu));
        printf("Response Length: %u\n", pdu->payload_len);
        printf("Response Payload: %.*s\n", pdu->payload_len, (char *)pdu->payload);
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid server IP address");
        return 1;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    gcoap_req_send(buf, pdu.payload_pos, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
