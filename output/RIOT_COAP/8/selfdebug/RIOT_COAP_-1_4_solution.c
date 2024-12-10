#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

// Function prototypes
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    
    // Initialize UDP remote endpoint
    remote.family = AF_INET;
    remote.port = 5683;
    
    // Convert IP address
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    // Initialize a POST request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    
    // Set payload
    const char *msg = "Hello, COAP Cloud";
    memcpy(pdu.payload, msg, strlen(msg));
    pdu.payload_len = strlen(msg);

    // Send the request
    gcoap_req_send(buf, pdu.payload_len + (pdu.payload - buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu == NULL) {
        printf("No response received\n");
    } else {
        printf("Response received; code: %u\n", pdu->hdr->code);
    }
}