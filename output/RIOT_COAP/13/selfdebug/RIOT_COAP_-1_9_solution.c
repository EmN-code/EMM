#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/sock/tcp.h"
#include "net/sock.h"
#include "net/ipv4/addr.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

// CoAP message buffer
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

// Function to handle the response
void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    // Define and initialize the remote sock_udp_ep_t structure
    sock_udp_ep_t remote = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF, .port = 5683 };
    
    // Convert the server IP
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Failed to convert IP address\n");
        return -1;
    }
    
    // Define the CoAP packet
    coap_pkt_t pdu;
    
    // Initialize the request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD)); // Add payload
    
    // Send the request
    ssize_t res = gcoap_req_send(buf, CONFIG_GCOAP_PDU_BUF_SIZE, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    
    if (res < 0) {
        printf("Failed to send CoAP request\n");
        return -1;
    }
    
    printf("CoAP request sent successfully\n");

    return 0;
}