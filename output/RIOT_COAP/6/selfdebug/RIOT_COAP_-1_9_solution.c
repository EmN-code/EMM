#include <string.h>  // Required for string manipulation functions like strlen and memcpy
#include <stdio.h>   // Required for standard input and output
#include <stdint.h>  // Required for standard integer types
#include <stdlib.h>  // Required for general utilities
#include "net/gcoap.h"
#include "net/sock/udp.h"  // Required for UDP operations

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main() {
    sock_udp_ep_t remote;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t result;
    
    // Convert the server IP address
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        puts("Error: unable to convert IP address");
        return -1;
    }
    remote.port = 5683; // Setting the port to 5683

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Sending the CoAP request
    result = gcoap_req_send(buf, sizeof(buf), &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        puts("Error: unable to send CoAP request");
        return -1;
    }

    puts("CoAP POST request sent successfully");
    return 0;
}