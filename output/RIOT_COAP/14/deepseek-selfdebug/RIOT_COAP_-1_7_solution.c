
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_RESP) {
        char resp[CONFIG_GCOAP_PDU_BUF_SIZE];
        size_t len = gcoap_resp_init(pdu, resp, sizeof(resp), COAP_CODE_CONTENT);
        if (len > 0) {
            printf("Received response: %s\n", resp);
        }
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid IP address");
        return 1;
    }

    // Initialize CoAP request
    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        puts("Failed to initialize CoAP request");
        return 1;
    }

    // Add payload to the CoAP request
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    pdu.payload_len = strlen(MESSAGE);

    // Send the CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
