#include <string.h>  // Ensure this is included for string handling functions
#include <stdio.h>   
#include "net/ipv6/addr.h"
#include "net/ipv4/addr.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define COAP_PORT 5683
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Simple response handler implementation
    if (memo->state == GCOAP_MEMO_RESP) {
        puts("Response received");
    } else {
        puts("Error in response");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };
    
    // Convert IPv4 string to address
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse IPv4 address");
        return 1;
    }
    
    // Initialize CoAP PDU
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Set payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    gcoap_req_send(&buf[0], pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    puts("CoAP request sent!");
    return 0;
}