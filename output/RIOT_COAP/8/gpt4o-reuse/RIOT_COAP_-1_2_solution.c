
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else if (memo->state == GCOAP_MEMO_RESP) {
        char *resp_payload = (char *)gcoap_get_payload(pdu, NULL);
        if (resp_payload) {
            printf("Received response: %s\n", resp_payload);
        }
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t len;

    /* Set up the remote endpoint */
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    /* Initialize the CoAP request */
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        puts("Failed to initialize CoAP request");
        return 1;
    }

    /* Add payload to the request */
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    if (coap_opt_add_format(&pdu, COAP_FORMAT_TEXT_PLAIN) < 0 ||
        coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD) < 0) {
        puts("Failed to add payload to CoAP request");
        return 1;
    }
    memcpy(pdu.payload, payload, payload_len);
    pdu.payload_len = payload_len;

    /* Send the request */
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Failed to send CoAP request");
        return 1;
    }

    /* Run the RIOT event loop */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
