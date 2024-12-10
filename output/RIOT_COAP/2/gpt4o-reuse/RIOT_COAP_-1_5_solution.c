
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
    /* Prepare the CoAP request */
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET };
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    remote.port = COAP_PORT;

    /* Initialize the request */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    coap_add_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 16);

    /* Send the request */
    ssize_t bytes_sent = gcoap_req_send(&buf[0], coap_get_total_size(&pdu), &remote, message_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    /* Run the CoAP event loop */
    while (1) {
        gcoap_event_loop();
    }

    return 0;
}
