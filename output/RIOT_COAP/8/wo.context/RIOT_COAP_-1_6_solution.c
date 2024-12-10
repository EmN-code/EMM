#include <net/sock/udp.h>
#include <net/gnrc.h>
#include <net/gcoap.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"  // The IP address of the CoAP server
#define SERVER_PORT 5683          // The CoAP server's port
#define URI_PATH "/coap/test"    // The URI path of the CoAP request
#define PAYLOAD "Hello, COAP Cloud" // The payload to send in the POST request

// Function to handle CoAP response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    (void)remote;
    (void)memo;
    if (pdu != NULL) {
        printf("[CoAP] Response code: %d\n", coap_get_code(pdu));
    } else {
        printf("[CoAP] No response received.\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    size_t len;

    // Initialize the remote endpoint structure
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize CoAP PDU packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len = coap_hdr_len(&pdu) + strlen(PAYLOAD);

    // Send CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    printf("[CoAP] Request sent: %s\n", PAYLOAD);

    return 0;
}
