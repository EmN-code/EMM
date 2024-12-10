#include <string.h>  // For string manipulation functions like memcpy
#include <stdio.h>   // For standard I/O functions like printf
#include "net/sock/udp.h"   // For UDP socket functions
#include "net/ipv6/addr.h"  // For IP address functions

#define SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define URI_PATH "/coap/test"

int main(void) {
    sock_udp_ep_t remote;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;
    
    // Convert IP address string to byte-represented IP address
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error converting IP address\n");
        return -1;
    }
    
    // Configure the remote endpoint
    remote.family = AF_INET;
    remote.port = COAP_PORT;

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    
    // Set the payload
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    
    // Send the request
    gcoap_req_send(buf, pdu.payload - (uint8_t *)buf + strlen(payload), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}