
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
    else if (memo->state == GCOAP_MEMO_RESP) {
        char *resp_buf = (char *)pdu->payload;
        size_t resp_len = pdu->payload_len;
        printf("Received response: %.*s\n", (int)resp_len, resp_buf);
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len;

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid server IP address");
        return 1;
    }

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Add payload to the request
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    // Send the request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Run the RIOT event loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}