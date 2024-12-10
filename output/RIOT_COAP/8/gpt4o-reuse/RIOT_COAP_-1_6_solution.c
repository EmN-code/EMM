
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // Include this for inet_pton
#include "gcoap.h"      // Include this for gcoap functions

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
    sock_udp_ep_t remote = { .family = AF_INET };

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4) <= 0) {
        perror("inet_pton");
        return 1;
    }

    remote.port = htons(5683);

    // Initialize the CoAP request
    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test") < 0) {
        perror("gcoap_req_init");
        return 1;
    }

    // Add payload to the request
    const char *payload = "Hello, COAP Cloud";
    coap_add_payload(&pdu, (uint8_t *)payload, strlen(payload));

    // Send the request
    gcoap_req_send(buf, coap_get_total_size(&pdu), &remote, message_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Run the CoAP event loop
    gcoap_event_loop();

    return 0;
}
