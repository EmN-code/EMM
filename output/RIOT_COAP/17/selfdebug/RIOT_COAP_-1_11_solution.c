#include <stdio.h>
#include <stdint.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/ipv4/addr.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE      (128)
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"

int main(void) {
    sock_udp_ep_t remote = { .port = CONFIG_GCOAP_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    
    // Convert IP string to network format
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error: Unable to convert IP address\n");
        return -1;
    }
    
    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    
    // Add payload
    size_t len = gcoap_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 17);
    if (len == 0) {
        printf("Error: Payload too large\n");
        return -1;
    }

    // Send the request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: Sending CoAP request failed\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}
