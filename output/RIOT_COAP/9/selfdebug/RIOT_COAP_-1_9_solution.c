#include <string.h>
#include "net/sock/udp.h"
#include "kernel_defines.h"
#include "net/ipv4/addr.h"

// Define the CoAP server settings
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"

// Define the message to send
#define MESSAGE "Hello, COAP Cloud"

// Function declaration
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void)
{
    // Setting up the remote endpoint
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;
    
    // Convert server IP address from text to binary form
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Invalid IP address format.\n");
        return -1;
    }

    // Prepare the CoAP PDU
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    coap_hdr_set_type(&pdu, COAP_TYPE_NON);
    
    size_t payload_offset = coap_get_payload(&pdu, (uint8_t **)&buf);
    memcpy(&buf[payload_offset], MESSAGE, strlen(MESSAGE));

    size_t len = coap_hdr_get_len(&pdu) + strlen(MESSAGE);

    // Send the CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}

// Response handler for the CoAP request
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu == NULL) {
        printf("Error in CoAP response
");
        return;
    }

    printf("Received CoAP response: %u\n", coap_hdr_get_code(pdu));
}