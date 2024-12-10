#include <stdio.h>  // For printf and other standard utilities
#include <string.h> // For handling string operations
#include <net/sock/udp.h> // For UDP socket functions
#include <net/gcoap.h> // For CoAP related functions and types

#define SERVER_IP "47.102.103.1" // COAP server IP
#define SERVER_PORT 5683 // COAP server port
#define URI_PATH "/coap/test" // URI path

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received from the COAP server\n"); // Placeholder implementation
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    // Set the payload for the CoAP message
    char *data = "Hello, COAP Cloud";
    memcpy(coap_payload(&pdu), data, strlen(data));
    pdu.payload_len = strlen(data);

    // Specify the remote server's IP and port
    remote.family = AF_INET6;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6);
    remote.port = SERVER_PORT;

    // Send the CoAP request
    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}