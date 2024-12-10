#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128  // Ensure this isn't redefined once

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

gcoap_response_handler_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int res;

    // Configure remote server
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;
    
    // Use inet_pton to convert the string IP address to binary form
    res = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        if (res == 0)
            puts("Invalid address format");
        else 
            puts("Address conversion error");
        return 1;  // Terminate due to conversion error
    }

    // Initialize the CoAP PDU for a POST request
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    
    // Add payload "Hello, COAP Cloud"
    memcpy(pdu.payload, "Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);
    len += sizeof("Hello, COAP Cloud") - 1;

    // Send the CoAP request
    ssize_t result = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        puts("Failed to send CoAP request");
        return 1;
    }
    
    puts("CoAP request sent successfully");
    return 0;
}

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state != GCOAP_MEMO_RESP_RECEIVED) {
        puts("Failed to receive response");
        return;
    }

    puts("Received response from CoAP server");
}