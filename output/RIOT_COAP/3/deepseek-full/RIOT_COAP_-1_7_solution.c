
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    } else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    } else {
        puts("Request succeeded");
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Invalid server IP address");
        return 1;
    }

    // Initialize CoAP packet
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH) == -1) {
        puts("Failed to initialize CoAP packet");
        return 1;
    }

    // Add payload to the CoAP packet
    if (coap_opt_add_format(&pdu, COAP_FORMAT_TEXT) == -1) {
        puts("Failed to add format option");
        return 1;
    }
    if (coap_add_payload(&pdu, PAYLOAD, strlen(PAYLOAD)) == -1) {
        puts("Failed to add payload");
        return 1;
    }

    // Finish the CoAP packet
    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE) == -1) {
        puts("Failed to finish CoAP packet");
        return 1;
    }

    // Send the CoAP request
    ssize_t len = coap_get_total_pkt_len(&pdu);
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) == -1) {
        puts("Failed to send CoAP request");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}