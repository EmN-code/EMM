
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void)
{
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    // Initialize CoAP request
    if (gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH) < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    // Add payload to the CoAP request
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    if (coap_add_payload(&pdu, (uint8_t *)payload, payload_len) < 0) {
        printf("Failed to add payload to CoAP request\n");
        return 1;
    }

    // Finish the CoAP request
    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE) < 0) {
        printf("Failed to finish CoAP request\n");
        return 1;
    }

    // Send the CoAP request
    if (gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");
    return 0;
}