#include <string.h>  // For strncpy functions
#include <net/sock/udp.h>  // Provides UDP sock structure
#include <net/nanocoap.h>  // Enables CoAP operations

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle response from server
    if (pdu != NULL) {
        printf("Response received with code %u\n", coap_get_code(pdu));
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_IP);  // Correcting the target address parsing

    /* Initialize and prepare the CoAP request */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    size_t len = strlen(COAP_PAYLOAD);
    strncpy((char *)pdu.payload, COAP_PAYLOAD, len);
    pdu.payload_len = len;

    /* Send the request */
    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Unable to send request\n");
        return -1;
    }

    printf("Request sent\n");
    return 0;
}