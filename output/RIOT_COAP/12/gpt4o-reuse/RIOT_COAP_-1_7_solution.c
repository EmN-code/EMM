
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

    if (coap_get_data(pdu, &data_len, &data))
    {
        printf("Received: %s\n", data);
    }
}

int main(void)
{
    /* Prepare the CoAP client socket */
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    /* Prepare the request */
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");
    if (len < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    /* Send the request */
    gcoap_req_send(buf, len, &remote, message_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    /* Run the event loop */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
