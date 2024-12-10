
#include <stdio.h>
#include <netinet/in.h>
#include "gcoap.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    if (pdu->hdr->code == COAP_CODE_CONTENT) {
        uint8_t *payload = coap_get_payload(pdu);
        size_t payload_len = coap_get_payload_len(pdu);
        printf("Received response: %.*s\n", (int)payload_len, payload);
    } else {
        printf("Error in response\n");
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    /* Convert IP address string to binary form */
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    remote.port = SERVER_PORT;

    /* Initialize CoAP request */
    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        puts("Error: cannot initialize CoAP request");
        return 1;
    }

    /* Add payload to the request */
    const char *payload = "Hello, COAP Cloud";
    len = coap_add_payload(&pdu, (uint8_t *)payload, strlen(payload));
    if (len < 0) {
        puts("Error: cannot add payload to CoAP request");
        return 1;
    }

    /* Send the request */
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    /* Run the RIOT event loop */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
