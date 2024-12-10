
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
    sock_udp_ep_t remote;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    /* Prepare the remote endpoint */
    result = inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    if (result != 1) {
        puts("Error: Invalid IP address");
        return 1;
    }
    remote.port = ntohs(5683);
    remote.family = AF_INET;

    /* Prepare the request */
    result = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    if (result < 0) {
        puts("Error: Failed to initialize CoAP request");
        return 1;
    }

    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));

    /* Send the request */
    gcoap_req_send(buf, result + strlen(payload), &remote, message_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    /* Run the RIOT event loop */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
