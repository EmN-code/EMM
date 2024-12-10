
#include <stdio.h>
#include <stdlib.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    } else {
        char *resp_buf = (char *)pdu->payload;
        printf("Received response: %s\n", resp_buf);
    }
}

int main(void)
{
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid IP address");
        return 1;
    }

    // Initialize CoAP request
    if (gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH) < 0) {
        puts("Failed to initialize CoAP request");
        return 1;
    }

    // Set payload
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    if (coap_pkt_set_payload(&pdu, (uint8_t *)payload, payload_len) < 0) {
        puts("Failed to set CoAP payload");
        return 1;
    }

    // Finish CoAP options
    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE) < 0) {
        puts("Failed to finish CoAP options");
        return 1;
    }

    // Send CoAP request
    if (gcoap_req_send(buf, pdu.offset, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Failed to send CoAP request");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}
