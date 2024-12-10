// Required header files for configuration and CoAP functionalities
#include <net/sock/udp.h>
#include <net/gnrc.h>
#include <stdio.h>
#include <string.h>

// Define CoAP server endpoint parameters
#define COAP_PORT 5683
#define COAP_METHOD_POST COAP_REQ_POST
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"

// Define buffer size for CoAP
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

// Dummy handler for response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received\n");
    }
    else {
        printf("Failed to receive response\n");
    }
}

int main(void) {
    // Set up remote endpoint
    sock_udp_ep_t remote = { .family = AF_INET6, .port = COAP_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    // Create CoAP PDU
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (len == 0) {
        printf("Failed to initialize CoAP request\n");
        return -1;
    }

    // Add payload to PDU
    char *payload = "Hello, COAP Cloud";
    coap_payload(pdu, (uint8_t *)payload, strlen(payload));

    // Send CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Failed to send CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}