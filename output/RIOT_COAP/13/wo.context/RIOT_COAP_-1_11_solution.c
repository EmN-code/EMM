#include <net/sock/udp.h>  // Ensure UDP socket functionality
#include <arpa/inet.h>      // Use inet_pton
#include <net/gcoap.h>      // Include gcoap features
#include <stdio.h>           // Standard IO - printf, etc.
#include <string.h>          // For memcpy, strcmp, etc.

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define LOCAL_PORT 5683

// Define CoAP server details and PDU buffer size
#define CONFIG_GCOAP_PDU_BUF_SIZE 256

// Define message characteristics
#define COAP_METHOD_POST 0x02
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu, sock_udp_ep_t *remote){
    if (req_state == GCOAP_MEMO_RESP_TRUNC) {
        printf("Response was truncated\n");
    }
    else if (req_state != GCOAP_MEMO_TIMEOUT) {
        printf("Received: code=%1u\n", coap_get_code(pdu));
    }
    else {
        printf("Timeout\n");
    }
}

int main(void)
{
    // Setup remote server endpoint
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Prepare CoAP PDU buffer
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Add payload
    const char *message = "Hello, COAP Cloud";
    memcpy(pdu.payload, message, strlen(message));
    pdu.payload_len = strlen(message);

    // Send the CoAP request
    if (gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}
