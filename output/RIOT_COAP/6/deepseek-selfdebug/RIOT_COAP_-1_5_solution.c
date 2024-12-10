
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
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request error");
    }
    else if (memo->state == GCOAP_MEMO_RESP) {
        char resp_buf[CONFIG_GCOAP_PDU_BUF_SIZE];
        size_t len = gcoap_response(pdu, (uint8_t *)resp_buf, sizeof(resp_buf), COAP_CODE_CONTENT);
        if (len > 0) {
            resp_buf[len] = '\0';
            printf("Received response: %s\n", resp_buf);
        }
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid server IP address");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        puts("Failed to initialize CoAP request");
        return 1;
    }

    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    pdu.payload_len = strlen(MESSAGE);

    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (len < 0) {
        puts("Failed to finish CoAP request");
        return 1;
    }

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Failed to send CoAP request");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}
