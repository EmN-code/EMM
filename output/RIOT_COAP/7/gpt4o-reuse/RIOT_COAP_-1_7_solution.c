
#include "gcoap.h"
#include <stdio.h>
#include <netinet/in.h>

/*
 * The response handler
 */ 
static void
message_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) 
{
    unsigned char* data;
    size_t data_len;

    if (pdu->payload_len > 0) {
        printf("Received: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
    }
}

int main(void)
{
    /* Prepare the CoAP client socket */
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    /* Prepare the request */
    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");
    if (len <= 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    /* Send the request */
    gcoap_req_send(buf, len, &remote, message_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    /* Run the RIOT event loop */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
