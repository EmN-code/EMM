#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/nanocoap/cache.h>
#include <thread_config.h>
#include "gcoap.h"

#define COAP_SERVER "47.102.103.1"
#define COAP_PORT CONFIG_GCOAP_PORT
#define COAP_URI "/coap/test"

// Define a buffer size for the CoAP message
#define COAP_BUF_SIZE 128

// Define the payload message
static const char *message = "Hello, COAP Cloud";

int main(void) {
    // Initialize buffer for the CoAP message
    uint8_t buf[COAP_BUF_SIZE];
    coap_pkt_t pdu;
  
    // Initialize a UDP endpoint for the server
    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = COAP_PORT
    };
    // Convert the server IP address to a network format
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER);

    // Initialize CoAP request PDU
    int len = gcoap_req_init(&pdu, buf, COAP_BUF_SIZE, COAP_METHOD_POST, COAP_URI);
    if (len < 0) {
        printf("Error: unable to initialize CoAP request\n");
        return 1;
    }

    // Set the payload for the CoAP request
    memcpy(pdu.payload, message, strlen(message));
    len += strlen(message);
    pdu.payload_len = strlen(message);

    // Send the CoAP request
    if (gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error: unable to send CoAP request\n");
        return 1;
    }

    printf("CoAP POST request sent successfully\n");
    return 0;
}