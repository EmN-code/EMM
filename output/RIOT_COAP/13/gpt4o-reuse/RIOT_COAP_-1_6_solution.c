
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
    /* Prepare the CoAP client socket */
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    /* Prepare the request */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    /* Send the request */
    ssize_t len = gcoap_req_send(buf, coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE), &remote, message_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len < 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    /* Run the CoAP event loop */
    while (1) {
        gcoap_event_loop();
    }

    return 0;
}
