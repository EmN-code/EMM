
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

int main(int argc, char* argv[])
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int result;

    /* Prepare the remote endpoint */
    remote.family = AF_INET;
    remote.port = htons(5683);
    result = inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    if (result != 1)
    {
        printf("Invalid IP address\n");
        return 1;
    }

    /* Prepare the CoAP request */
    result = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    if (result < 0)
    {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    result = coap_add_payload(&pdu, (uint8_t *)payload, payload_len);
    if (result < 0)
    {
        printf("Failed to add payload to CoAP request\n");
        return 1;
    }

    /* Send the CoAP request */
    result = gcoap_req_send(buf, result, &remote, message_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result < 0)
    {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    /* Run the CoAP event loop */
    while (1)
    {
        gcoap_event_loop();
    }

    return 0;
}
