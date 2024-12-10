#include "string.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "arpa/inet.h"

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT    5683
#define URI_PATH       "/coap/test"
#define MESSAGE        "Hello, COAP Cloud"

static void _response_handler(const gcoap_pkt_t *pdu, const sock_udp_ep_t *remote, void *context) {
    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    printf("Sending CoAP POST request\n");

    // Convert the string IP address to a network format IP address
    if (inet_pton(AF_INET, SERVER_ADDRESS, &remote.addr.ipv4) != 1) {
        printf("Failed to convert address\n");
        return 1;
    }

    // Initialize CoAP PDU
    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH) < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    // Add payload
    if (coap_opt_add_payload(&pdu, (uint8_t *)MESSAGE, strlen(MESSAGE)) < 0) {
        printf("Failed to add payload\n");
        return 1;
    }

    ssize_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    if (gcoap_req_send(buf, len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");
    return 0;
}
